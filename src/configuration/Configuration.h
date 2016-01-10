#ifndef LIGHTER_CONFIGURATION_H
#define LIGHTER_CONFIGURATION_H

#include <string>

class Configuration {
public:
    int serverPort = 8080;
    std::string dataPath = "./data/";
    std::string deviceTypesPath = "./types/";
    std::string devicesPath = "./devices.json";

    std::string toString();
};


#endif //LIGHTER_CONFIGURATION_H
