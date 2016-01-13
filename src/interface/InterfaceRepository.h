#ifndef LIGHTER_INTERFACEREPOSITORY_H
#define LIGHTER_INTERFACEREPOSITORY_H

#include "Interface.h"
#include "ArtNetInterface.h"
#include <map>

class InterfaceRepository {
public:
    bool loadFromFile(std::string path);
    ArtNetInterface* findByUniverse(int universe);

private:
    std::map<int, ArtNetInterface*> interfaces;
};


#endif //LIGHTER_INTERFACEREPOSITORY_H
