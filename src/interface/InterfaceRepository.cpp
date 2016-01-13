#include "InterfaceRepository.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#include "ArtNetInterface.h"
#include <easylogging++.h>

bool InterfaceRepository::loadFromFile(std::string path) {
    FILE* filePointer = fopen(path.c_str(), "r");

    if(filePointer == NULL) {
        LOG(ERROR) << "Cant't open interfaces configuration file " << path;
        return NULL;
    }

    char buffer[65536];
    rapidjson::FileReadStream inputStream(filePointer, buffer, sizeof(buffer));
    rapidjson::Document document;
    document.ParseStream(inputStream);
    fclose(filePointer);

    for(rapidjson::Value::ConstMemberIterator itr = document.MemberBegin(); itr != document.MemberEnd(); ++itr) {
        ArtNetInterface* interface = new ArtNetInterface();
        interface->id = itr->name.GetString();

        if(!itr->value.HasMember("name") || !itr->value["name"].IsString()) {
            LOG(ERROR) << "Cant't read property \"name\" from interfaces configuration in file " << path;
            return false;
        }
        interface->name = itr->value["name"].GetString();

        if(!itr->value.HasMember("universe") || !itr->value["universe"].IsInt()) {
            LOG(ERROR) << "Cant't read property \"universe\" from interfaces configuration in file " << path;
            return false;
        }
        interface->universe = itr->value["universe"].GetInt();

        if(!itr->value.HasMember("address") || !itr->value["address"].IsString()) {
            LOG(ERROR) << "Cant't read property \"address\" from interfaces configuration in file " << path;
            return false;
        }
        interface->address = itr->value["address"].GetString();

        interface->init();
        this->interfaces[interface->universe] = interface;
        return true;
    }
}

ArtNetInterface *InterfaceRepository::findByUniverse(int universe) {
    if(this->interfaces.find(universe) != this->interfaces.end()) {
        return this->interfaces.find(universe)->second;
    }

    return NULL;
}
