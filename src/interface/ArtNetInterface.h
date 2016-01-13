#ifndef LIGHTER_ARTNETINTERFACE_H
#define LIGHTER_ARTNETINTERFACE_H

#include "../dmx/DmxUniverse.h"
#include "../interface/Interface.h"
#include <string>

class ArtNetInterface : public Interface {
public:
    void sendDmxUniverse(DmxUniverse* universe);
    void init();

private:
    struct addrinfo * f_addrinfo;
    int sock;
    char sequenceNumber;
};


#endif //LIGHTER_ARTNETINTERFACE_H
