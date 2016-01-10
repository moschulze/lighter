#ifndef LIGHTER_DEVICE_H
#define LIGHTER_DEVICE_H


#include <string>
#include "DeviceType.h"

class Device {
public:
    Device(DeviceType* type);
    std::string id;
    std::string name;
    DeviceType* type;
    int universe = 0;
    int startAddress = 0;

    int* slotData;
};


#endif //LIGHTER_DEVICE_H
