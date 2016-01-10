#ifndef LIGHTER_DEVICETYPEREPOSITORY_H
#define LIGHTER_DEVICETYPEREPOSITORY_H

#include "DeviceType.h"
#include <string>

class DeviceTypeRepository {
public:
    DeviceTypeRepository(std::string directory);
    DeviceType* findById(std::string id);

private:
    std::string directory = "";
    std::map<std::string, DeviceType*> types;
};


#endif //LIGHTER_DEVICETYPEREPOSITORY_H
