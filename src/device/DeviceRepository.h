#ifndef LIGHTER_DEVICEREPOSITORY_H
#define LIGHTER_DEVICEREPOSITORY_H

#include <string>
#include <map>
#include "Device.h"
#include "DeviceTypeRepository.h"
#include <vector>

class DeviceRepository {
public:
    DeviceRepository(DeviceTypeRepository* deviceTypeRepository);
    bool loadFromFile(std::string path);
    Device* findById(std::string id);
    std::vector<int> getUniverses();

private:
    std::map<std::string, Device*> devices;
    DeviceTypeRepository* deviceTypeRepository;
};


#endif //LIGHTER_DEVICEREPOSITORY_H
