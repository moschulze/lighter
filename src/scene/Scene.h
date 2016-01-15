#ifndef LIGHTER_SCENE_H
#define LIGHTER_SCENE_H

#include <string>
#include <map>
#include <vector>
#include "SceneStep.h"

class Scene {
public:
    std::string id;
    std::string name;
    void addStep(SceneStep* step);
    SceneStep* getFirstStep();
    SceneStep* getStep(std::string id);
    std::string activeStep = "";
    std::map<std::string, SceneStep*>::iterator stepsBegin();
    std::map<std::string, SceneStep*>::iterator stepsEnd();
    std::vector<Device*> getAllDevices();

private:
    std::map<std::string, SceneStep*> steps;
};


#endif //LIGHTER_SCENE_H
