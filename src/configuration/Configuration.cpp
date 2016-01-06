#include "Configuration.h"

#include <sstream>

std::string Configuration::toString() {
    std::stringstream returnString;

    returnString << "server.port: " << this->serverPort << std::endl;
    returnString << "data.path: " << this->dataPath << std::endl;

    return returnString.str();
}
