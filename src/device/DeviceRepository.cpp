#include "DeviceRepository.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#include <easylogging++.h>
#include <algorithm>

DeviceRepository::DeviceRepository(DeviceTypeRepository *deviceTypeRepository) {
    this->deviceTypeRepository = deviceTypeRepository;
}

bool DeviceRepository::loadFromFile(std::string path) {
    FILE* filePointer = fopen(path.c_str(), "r");

    if(filePointer == NULL) {
        LOG(ERROR) << "Cant't open devices configuration file " << path;
        return NULL;
    }

    char buffer[65536];
    rapidjson::FileReadStream inputStream(filePointer, buffer, sizeof(buffer));
    rapidjson::Document document;
    document.ParseStream(inputStream);
    fclose(filePointer);

    for(rapidjson::Value::ConstMemberIterator itr = document.MemberBegin(); itr != document.MemberEnd(); ++itr) {
        if(!itr->value.HasMember("type") || !itr->value["type"].IsString()) {
            LOG(ERROR) << "Cant't read property \"type\" from devices configuration in file " << path;
            return false;
        }

        DeviceType* type = this->deviceTypeRepository->findById(itr->value["type"].GetString());
        if(type == NULL) {
            LOG(ERROR) << "Cant't load device type \"" << itr->value["type"].GetString() << "\"";
            return false;
        }

        Device* device = new Device(type);
        device->id = itr->name.GetString();

        if(!itr->value.HasMember("name") || !itr->value["name"].IsString()) {
            LOG(ERROR) << "Cant't read property \"name\" from devices configuration in file " << path;
            return false;
        }
        device->name = itr->value["name"].GetString();

        if(!itr->value.HasMember("universe") || !itr->value["universe"].IsInt()) {
            LOG(ERROR) << "Cant't read property \"universe\" from devices configuration in file " << path;
            return false;
        }
        device->universe = itr->value["universe"].GetInt();

        if(!itr->value.HasMember("startAddress") || !itr->value["startAddress"].IsInt()) {
            LOG(ERROR) << "Cant't read property \"startAddress\" from devices configuration in file " << path;
            return false;
        }
        device->startAddress = itr->value["startAddress"].GetInt();

        this->devices[device->id] = device;
    }

    return true;
}

Device *DeviceRepository::findById(std::string id) {
    if(this->devices.find(id) != this->devices.end()) {
        return this->devices.find(id)->second;
    }

    return NULL;
}

std::vector<int> DeviceRepository::getUniverses() {
    std::vector<int> universes;

    for(std::map<std::string, Device*>::iterator itr = this->devices.begin(); itr != this->devices.end(); itr++) {
        if(std::find(universes.begin(), universes.end(), itr->second->universe) == universes.end()) {
            universes.push_back(itr->second->universe);
        }
    }

    return universes;
}
