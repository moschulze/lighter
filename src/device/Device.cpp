#include "Device.h"

Device::Device(DeviceType *type) {
    this->type = type;
    this->slotData = new int[type->getNumberOfSlots()];
}
