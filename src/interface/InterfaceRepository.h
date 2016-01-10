#ifndef LIGHTER_INTERFACEREPOSITORY_H
#define LIGHTER_INTERFACEREPOSITORY_H

#include "Interface.h"
#include <map>

class InterfaceRepository {
public:
    bool loadFromFile(std::string path);
    Interface* findByUniverse(int universe);

private:
    std::map<int, Interface*> interfaces;
};


#endif //LIGHTER_INTERFACEREPOSITORY_H
