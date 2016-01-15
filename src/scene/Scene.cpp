#include "Scene.h"
#include <algorithm>

void Scene::addStep(SceneStep *step) {
    this->steps[step->id] = step;
}

SceneStep *Scene::getStep(std::string id) {
    if(this->steps.find(id) != this->steps.end()) {
        return this->steps.find(id)->second;
    }

    return NULL;
}

SceneStep *Scene::getFirstStep() {
    return this->steps.begin()->second;
}

std::map<std::string, SceneStep *>::iterator Scene::stepsBegin() {
    return this->steps.begin();
}

std::map<std::string, SceneStep *>::iterator Scene::stepsEnd() {
    this->steps.end();
}

std::vector<Device *> Scene::getAllDevices() {
    std::vector<Device *> devices;

    for(auto stepItr = this->steps.begin(); stepItr != this->steps.end(); stepItr++) {
        SceneStep* step = stepItr->second;
        for(auto deviceItr = step->data.begin(); deviceItr != step->data.end(); deviceItr++) {
            Device* device = deviceItr->first;
            if(std::find(devices.begin(), devices.end(), device) == devices.end()) {
                devices.push_back(device);
            }
        }
    }

    return devices;
}
