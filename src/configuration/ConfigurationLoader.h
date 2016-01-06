#ifndef LIGHTER_CONFIGURATIONLOADER_H
#define LIGHTER_CONFIGURATIONLOADER_H


#include "Configuration.h"

class ConfigurationLoader {
public:
    Configuration* loadFromFile(char* path);
};


#endif //LIGHTER_CONFIGURATIONLOADER_H
