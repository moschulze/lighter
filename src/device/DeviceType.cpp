#include "DeviceType.h"

int DeviceType::getNumberOfSlots() {
    return this->slots.size();
}

void DeviceType::addSlot(std::string name, int number) {
    this->slots[name] = number;
}
