#include "DeviceTypeRepository.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#include <easylogging++.h>

DeviceTypeRepository::DeviceTypeRepository(std::string directory) {
    this->directory = directory;
}

DeviceType *DeviceTypeRepository::findById(std::string id) {
    if(this->types.find(id) != this->types.end()) {
        VLOG(9) << "Loading device type from cache: " << id;
        return this->types.find(id)->second;
    }

    VLOG(9) << "Loading device type from file: " << id;

    std::string path = this->directory;
    path += id;
    path += ".json";

    FILE* filePointer = fopen(path.c_str(), "r");

    if(filePointer == NULL) {
        LOG(ERROR) << "Cant't open device type configuration file " << path;
        return NULL;
    }

    char buffer[65536];
    rapidjson::FileReadStream inputStream(filePointer, buffer, sizeof(buffer));
    rapidjson::Document document;
    document.ParseStream(inputStream);
    fclose(filePointer);

    DeviceType* type = new DeviceType();
    type->id = id;

    if(!document.HasMember("name") || !document["name"].IsString()) {
        LOG(ERROR) << "Cant't read property \"name\" from device type configuration in file " << path;
        return NULL;
    }
    type->name = document["name"].GetString();

    if(!document.HasMember("slots") || !document["slots"].IsObject()) {
        LOG(ERROR) << "Cant't read property \"slots\" from device type configuration in file " << path;
        return NULL;
    }
    for(rapidjson::Value::ConstMemberIterator itr = document["slots"].MemberBegin(); itr != document["slots"].MemberEnd(); ++itr) {
        if(!itr->name.IsString() || !itr->value.IsInt()) {
            LOG(ERROR) << "Invalid slot configuration in device type configuration file " << path;
            return NULL;
        }
        type->addSlot(itr->name.GetString(), itr->value.GetInt());
    }

    this->types[id] = type;

    return type;
}
