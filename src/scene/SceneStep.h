#ifndef LIGHTER_SCENESTEP_H
#define LIGHTER_SCENESTEP_H


#include <string>
#include <map>
#include "../device/Device.h"

class SceneStep {
public:
    std::string id;
    int fadeInTime;
    int fadeInAnimation;
    int duration;
    std::string next;
    void addDeviceData(Device* device, uint8_t* data);
    int passedFadeInTime;
    int passedDuration;

    std::map<Device*, uint8_t*> data;
    std::map<Device*, uint8_t*> oldData;

    int state;
    static const int STATE_FADE_IN = 0;
    static const int STATE_SHOW = 1;

    static const int FADE_LINEAR = 0;
    static const int FADE_SINUS = 1;
};


#endif //LIGHTER_SCENESTEP_H
