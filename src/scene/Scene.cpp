#include "Scene.h"

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
