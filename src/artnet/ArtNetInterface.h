#ifndef LIGHTER_ARTNETINTERFACE_H
#define LIGHTER_ARTNETINTERFACE_H

#include "../dmx/DmxUniverse.h"
#include <string>

class ArtNetInterface {
public:
    void sendDmxUniverse(DmxUniverse* universe);
    void init(std::string targetAddress, int port);

private:
    struct addrinfo * f_addrinfo;
    int sock;
    char sequenceNumber;
};


#endif //LIGHTER_ARTNETINTERFACE_H
