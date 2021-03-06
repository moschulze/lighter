#ifndef LIGHTER_RENDERER_H
#define LIGHTER_RENDERER_H

#include <map>
#include <string>
#include "../scene/Scene.h"
#include "../dmx/DmxUniverse.h"
#include "../interface/InterfaceRepository.h"
#include "../device/DeviceRepository.h"

class Renderer {
public:
    Renderer(InterfaceRepository* interfaceRepository);
    void start();
    void stop();
    void startScene(Scene* scene);
    void stopScene(Scene* scene);
    void init(DeviceRepository* deviceRepository);

private:
    std::map<std::string, Scene*> activeScenes;
    std::map<int, DmxUniverse*> universes;
    bool run = true;
    InterfaceRepository* interfaceRepository;
    void switchSceneStep(Scene* scene, std::string stepId);
    DmxUniverse* getUniverse(int id);
    void sendUniverses();
    void applySlotData(SceneStep* activeStep);
    void applySlotDataFaded(SceneStep* activeStep);
    bool renderFrame(int timePassed);
};


#endif //LIGHTER_RENDERER_H
