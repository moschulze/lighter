#ifndef LIGHTER_DEVICEREPOSITORY_H
#define LIGHTER_DEVICEREPOSITORY_H

#include <string>
#include <map>
#include "Device.h"
#include "DeviceTypeRepository.h"

class DeviceRepository {
public:
    DeviceRepository(DeviceTypeRepository* deviceTypeRepository);
    bool loadFromFile(std::string path);

private:
    std::map<std::string, Device*> devices;
    DeviceTypeRepository* deviceTypeRepository;
};


#endif //LIGHTER_DEVICEREPOSITORY_H
