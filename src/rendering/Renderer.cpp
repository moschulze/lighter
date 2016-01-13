#include "Renderer.h"
#include "ValueCalculator.h"
#include <chrono>
#include "unistd.h"

Renderer::Renderer(InterfaceRepository *interfaceRepository) {
    this->interfaceRepository = interfaceRepository;
}

void Renderer::start() {
    bool changed = false;
    std::chrono::milliseconds last = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    while(run) {
        std::chrono::milliseconds now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
        auto diffMs = now - last;
        int timePassed = diffMs.count();
        last = now;

        for(std::map<std::string, Scene*>::iterator itr = this->activeScenes.begin(); itr != this->activeScenes.end(); itr++) {
            Scene* scene = itr->second;
            SceneStep* activeStep = scene->getStep(scene->activeStep);

            if(activeStep->state == SceneStep::STATE_FADE_IN) {
                activeStep->passedFadeInTime += timePassed;

                if(activeStep->passedFadeInTime > activeStep->fadeInTime) {
                    activeStep->state = SceneStep::STATE_SHOW;

                    for(std::map<Device*, uint8_t*>::iterator dataItr = activeStep->data.begin(); dataItr != activeStep->data.end(); dataItr++) {
                        Device* device = dataItr->first;
                        uint8_t* data = dataItr->second;

                        DmxUniverse* universe = this->getUniverse(device->universe);
                        for(int i = 0; i < device->type->getNumberOfSlots(); i++) {
                            universe->setSlot(device->startAddress + i, data[i]);
                        }
                    }

                    continue;
                }

                for(std::map<Device*, uint8_t*>::iterator dataItr = activeStep->data.begin(); dataItr != activeStep->data.end(); dataItr++) {
                    Device* device = dataItr->first;
                    uint8_t* data = dataItr->second;
                    uint8_t* oldData = activeStep->oldData.find(device)->second;

                    DmxUniverse* universe = this->getUniverse(device->universe);
                    for(int i = 0; i < device->type->getNumberOfSlots(); i++) {
                        universe->setSlot(device->startAddress + i, ValueCalculator::calculate(oldData[i], data[i], activeStep->fadeInTime, activeStep->passedFadeInTime, activeStep->fadeInAnimation));
                    }
                }
                changed = true;
            } else if(activeStep->state == SceneStep::STATE_SHOW) {
                activeStep->passedDuration += timePassed;
                if(activeStep->passedDuration > activeStep->duration) {
                    this->switchSceneStep(scene, activeStep->next);
                }
            }
        }

        if(changed) {
            for(std::map<int, DmxUniverse*>::iterator itr = this->universes.begin(); itr != universes.end(); itr++) {
                this->interfaceRepository->findByUniverse(itr->first)->sendDmxUniverse(itr->second);
            }
        }

        int sleepTime = 33 - timePassed;
        if(sleepTime > 0) {
            usleep(sleepTime * 1000);
        }
    }
}

void Renderer::startScene(Scene *scene) {
    SceneStep* firstStep = scene->getFirstStep();
    this->switchSceneStep(scene, firstStep->id);
    this->activeScenes[scene->id] = scene;
}

void Renderer::switchSceneStep(Scene *scene, std::string stepId) {
    SceneStep* step = scene->getStep(stepId);
    step->passedDuration = 0;
    step->passedFadeInTime = 0;
    step->state = SceneStep::STATE_FADE_IN;

    scene->activeStep = stepId;

    for(std::map<Device*, uint8_t*>::iterator dataItr = step->data.begin(); dataItr != step->data.end(); dataItr++) {
        Device* device = dataItr->first;
        uint8_t* oldData = new uint8_t[device->type->getNumberOfSlots()];
        DmxUniverse* universe = this->getUniverse(device->universe);

        for(int i = 0; i < device->type->getNumberOfSlots(); i++) {
            oldData[i] = universe->getSlot(device->startAddress + i);
        }

        step->oldData[device] = oldData;
    }
}

DmxUniverse *Renderer::getUniverse(int id) {
    if(this->universes.find(id) == this->universes.end()) {
        this->universes[id] = new DmxUniverse();
    }

    return this->universes.find(id)->second;
}
