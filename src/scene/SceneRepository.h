#ifndef LIGHTER_SCENEREPOSITORY_H
#define LIGHTER_SCENEREPOSITORY_H

#include <string>
#include <map>
#include "Scene.h"
#include "../device/DeviceRepository.h"

class SceneRepository {
public:
    SceneRepository(DeviceRepository* deviceRepository);
    bool loadFromFile(std::string path);
    Scene* findById(std::string id);

private:
    DeviceRepository* deviceRepository;
    std::map<std::string, Scene*> scenes;
};


#endif //LIGHTER_SCENEREPOSITORY_H
