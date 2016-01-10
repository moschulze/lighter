#include "DmxUniverse.h"

DmxUniverse::DmxUniverse() {
    this->reset();
}

void DmxUniverse::setSlot(int slot, uint8_t value) {
    this->data[slot] = value;
}

uint8_t DmxUniverse::getSlot(int slot) {
    return this->data[slot];
}

void DmxUniverse::reset() {
    for(int i = 0; i < 512; i++) {
        this->data[i] = 0;
    }
}
