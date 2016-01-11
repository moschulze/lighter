#ifndef LIGHTER_SCENESTEP_H
#define LIGHTER_SCENESTEP_H


#include <string>
#include <map>
#include "../device/Device.h"

class SceneStep {
public:
    std::string id;
    int fadeInTime;
    std::string fadeInAnimation;
    int duration;
    std::string next;
    void addDeviceData(Device* device, uint8_t* data);

private:
    std::map<Device*, uint8_t*> data;
};


#endif //LIGHTER_SCENESTEP_H
