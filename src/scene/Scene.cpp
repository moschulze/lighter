#include "Scene.h"

void Scene::addStep(SceneStep *step) {
    this->steps[step->id] = step;
}
