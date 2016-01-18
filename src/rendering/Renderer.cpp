#include "Renderer.h"
#include "ValueCalculator.h"
#include <chrono>
#include "unistd.h"
#include <vector>

Renderer::Renderer(InterfaceRepository *interfaceRepository) {
    this->interfaceRepository = interfaceRepository;
}

void Renderer::init(DeviceRepository *deviceRepository) {
    std::vector<int> universes = deviceRepository->getUniverses();
    for(std::vector<int>::iterator itr = universes.begin(); itr != universes.end(); itr++) {
        this->universes[*itr] = new DmxUniverse();
    }
}

void Renderer::start() {
    std::chrono::milliseconds last = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    int sendTimeout = 0;
    bool changed;
    while(run) {
        std::chrono::milliseconds now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
        auto diffMs = now - last;
        int timePassed = diffMs.count();
        sendTimeout += timePassed;
        last = now;
        changed = false;
        if(sendTimeout >= 3000) {
            sendTimeout = 0;
            changed = true;
        }

        if(renderFrame(timePassed)) {
            changed = true;
        }

        if(changed) {
            this->sendUniverses();
        }

        int sleepTime = 33 - timePassed;
        if(sleepTime > 0) {
            usleep(sleepTime * 1000);
        }
    }

    for(std::map<int, DmxUniverse*>::iterator itr = this->universes.begin(); itr != universes.end(); itr++) {
        itr->second->reset();
    }
    this->sendUniverses();
}

bool Renderer::renderFrame(int timePassed) {
    bool changed = false;
    for(std::map<std::string, Scene*>::iterator itr = this->activeScenes.begin(); itr != this->activeScenes.end(); itr++) {
        Scene* scene = itr->second;
        SceneStep* activeStep = scene->getStep(scene->activeStep);

        if(activeStep->state == SceneStep::STATE_FADE_IN) {
            activeStep->passedFadeInTime += timePassed;

            if(activeStep->passedFadeInTime > activeStep->fadeInTime) {
                activeStep->state = SceneStep::STATE_SHOW;
                this->applySlotData(activeStep);
                changed = true;
                continue;
            }

            this->applySlotDataFaded(activeStep);
            changed = true;
        } else if(activeStep->state == SceneStep::STATE_SHOW) {
            if(activeStep->duration == -1) {
                continue;
            }

            activeStep->passedDuration += timePassed;
            if(activeStep->passedDuration > activeStep->duration) {
                if(activeStep->next.compare("") == 0) {
                    this->activeScenes.erase(scene->id);
                    continue;
                }

                this->switchSceneStep(scene, activeStep->next);
            }
        }
    }
    return changed;
}

void Renderer::startScene(Scene *scene) {
    if(this->activeScenes.find(scene->id) != this->activeScenes.end()) {
        return;
    }

    SceneStep* firstStep = scene->getFirstStep();
    this->switchSceneStep(scene, firstStep->id);
    this->activeScenes[scene->id] = scene;
}

void Renderer::stopScene(Scene *scene) {
    if(this->activeScenes.find(scene->id) == this->activeScenes.end()) {
        return;
    }

    this->switchSceneStep(scene, "end");
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

void Renderer::stop() {
    this->run = false;
}

void Renderer::sendUniverses() {
    for(std::map<int, DmxUniverse*>::iterator itr = this->universes.begin(); itr != universes.end(); itr++) {
        this->interfaceRepository->findByUniverse(itr->first)->sendDmxUniverse(itr->second);
    }
}

void Renderer::applySlotData(SceneStep *activeStep) {
    for(std::map<Device*, uint8_t*>::iterator dataItr = activeStep->data.begin(); dataItr != activeStep->data.end(); dataItr++) {
        Device* device = dataItr->first;
        uint8_t* data = dataItr->second;

        DmxUniverse* universe = this->getUniverse(device->universe);
        for(int i = 0; i < device->type->getNumberOfSlots(); i++) {
            universe->setSlot(device->startAddress + i, data[i]);
        }
    }
}

void Renderer::applySlotDataFaded(SceneStep *activeStep) {
    for(std::map<Device*, uint8_t*>::iterator dataItr = activeStep->data.begin(); dataItr != activeStep->data.end(); dataItr++) {
        Device* device = dataItr->first;
        uint8_t* data = dataItr->second;
        uint8_t* oldData = activeStep->oldData.find(device)->second;

        DmxUniverse* universe = this->getUniverse(device->universe);
        for(int i = 0; i < device->type->getNumberOfSlots(); i++) {
            universe->setSlot(device->startAddress + i, ValueCalculator::calculate(oldData[i], data[i], activeStep->fadeInTime, activeStep->passedFadeInTime, activeStep->fadeInAnimation));
        }
    }
}
