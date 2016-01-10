#ifndef LIGHTER_DMXUNIVERSE_H
#define LIGHTER_DMXUNIVERSE_H


#include <stdint-gcc.h>

class DmxUniverse {
public:
    DmxUniverse();
    void setSlot(int slot, uint8_t value);
    uint8_t getSlot(int slot);
    void reset();

private:
    uint8_t data[512];
};


#endif //LIGHTER_DMXUNIVERSE_H
