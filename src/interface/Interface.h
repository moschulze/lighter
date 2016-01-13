#ifndef LIGHTER_INTERFACE_H
#define LIGHTER_INTERFACE_H

#include <string>

class Interface {
public:
    std::string id;
    std::string name;
    int universe;
    int port = 6454;
    std::string address;
};


#endif //LIGHTER_INTERFACE_H
