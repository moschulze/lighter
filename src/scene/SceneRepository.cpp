#include "SceneRepository.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#include <easylogging++.h>
#include <map>
#include <vector>

SceneRepository::SceneRepository(DeviceRepository *deviceRepository) {
    this->deviceRepository = deviceRepository;
}

Scene *SceneRepository::findById(std::string id) {
    if(this->scenes.find(id) != this->scenes.end()) {
        return this->scenes.find(id)->second;
    }

    return NULL;
}

bool SceneRepository::loadFromFile(std::string path) {
    FILE* filePointer = fopen(path.c_str(), "r");

    if(filePointer == NULL) {
        LOG(ERROR) << "Cant't open scenes configuration file " << path;
        return NULL;
    }

    char buffer[65536];
    rapidjson::FileReadStream inputStream(filePointer, buffer, sizeof(buffer));
    rapidjson::Document document;
    document.ParseStream(inputStream);
    fclose(filePointer);

    for(rapidjson::Value::ConstMemberIterator scenesItr = document.MemberBegin(); scenesItr != document.MemberEnd(); ++scenesItr) {
        Scene* scene = new Scene();
        scene->id = scenesItr->name.GetString();

        if(!scenesItr->value.HasMember("name") || !scenesItr->value["name"].IsString()) {
            LOG(ERROR) << "Cant't read property \"name\" from scene configuration of " << scenesItr->name.GetString() << " in file " << path;
            return false;
        }
        scene->name = scenesItr->value["name"].GetString();

        if(!scenesItr->value.HasMember("steps") || !scenesItr->value["steps"].IsObject()) {
            LOG(ERROR) << "Cant't read property \"steps\" from scene configuration of scene " << scenesItr->name.GetString() << " in file " << path;
            return false;
        }
        for(rapidjson::Value::ConstMemberIterator stepsItr = scenesItr->value["steps"].MemberBegin(); stepsItr != scenesItr->value["steps"].MemberEnd(); ++stepsItr) {
            SceneStep* step = new SceneStep();
            step->id = stepsItr->name.GetString();

            if(!stepsItr->value.HasMember("fadeInTime") || !stepsItr->value["fadeInTime"].IsInt()) {
                LOG(ERROR) << "Cant't read property \"fadeInTime\" from scene configuration of scene " << scenesItr->name.GetString() << " step " << step->id <<" in file " << path;
                return false;
            }
            step->fadeInTime = stepsItr->value["fadeInTime"].GetInt();

            if(!stepsItr->value.HasMember("duration") || !stepsItr->value["duration"].IsInt()) {
                LOG(ERROR) << "Cant't read property \"duration\" from scene configuration of scene " << scenesItr->name.GetString() << " step " << step->id <<" in file " << path;
                return false;
            }
            step->duration = stepsItr->value["duration"].GetInt();

            if(!stepsItr->value.HasMember("fadeInAnimation") || !stepsItr->value["fadeInAnimation"].IsString()) {
                LOG(ERROR) << "Cant't read property \"fadeInAnimation\" from scene configuration of scene " << scenesItr->name.GetString() << " step " << step->id <<" in file " << path;
                return false;
            }
            step->fadeInAnimation = this->getFadeType(stepsItr->value["fadeInAnimation"].GetString());

            if(!stepsItr->value.HasMember("next") || !stepsItr->value["next"].IsString()) {
                LOG(ERROR) << "Cant't read property \"next\" from scene configuration of scene " << scenesItr->name.GetString() << " step " << step->id <<" in file " << path;
                return false;
            }
            step->next = stepsItr->value["next"].GetString();

            if(!stepsItr->value.HasMember("data") || !stepsItr->value["data"].IsObject()) {
                LOG(ERROR) << "Cant't read property \"data\" from scene configuration of scene " << scenesItr->name.GetString() << " step " << step->id <<" in file " << path;
                return false;
            }
            for(rapidjson::Value::ConstMemberIterator dataItr = stepsItr->value["data"].MemberBegin(); dataItr != stepsItr->value["data"].MemberEnd(); ++dataItr) {
                Device* device = this->deviceRepository->findById(dataItr->name.GetString());
                if(device == NULL) {
                    LOG(ERROR) << "Invalid device " << dataItr->name.GetString() << " in scene configuration of scene " << scenesItr->name.GetString() << " step " << step->id <<" in file " << path;
                    return false;
                }

                uint8_t* data = new uint8_t[device->type->getNumberOfSlots()];
                int i = 0;
                for(rapidjson::Value::ConstValueIterator valueItr = dataItr->value.Begin(); valueItr != dataItr->value.End(); valueItr++) {
                    data[i] = valueItr->GetInt();
                    i++;
                }

                step->addDeviceData(device, data);
            }

            scene->addStep(step);
        }

        if(!this->sceneHasEndStep(scene)) {
            scene->addStep(this->buildEndStepForScene(scene));
        }

        this->scenes[scene->id] = scene;
    }

    return true;
}

int SceneRepository::getFadeType(std::string type) {
    if(type.compare("linear") == 0) {
        return SceneStep::FADE_LINEAR;
    } else if(type.compare("sinus") == 0) {
        return SceneStep::FADE_SINUS;
    } else {
        return SceneStep::FADE_LINEAR;
    }
}

bool SceneRepository::sceneHasEndStep(Scene *scene) {
    for(auto itr = scene->stepsBegin(); itr != scene->stepsEnd(); itr++) {
        if(itr->second->id.compare("end") == 0) {
            return true;
        }
    }
    return false;
}

SceneStep *SceneRepository::buildEndStepForScene(Scene *scene) {
    SceneStep* step = new SceneStep();
    step->id = "end";
    step->duration = 0;
    step->fadeInTime = 0;
    step->fadeInAnimation = SceneStep::FADE_LINEAR;
    step->next = "";

    std::vector<Device*> devices = scene->getAllDevices();
    for(auto itr = devices.begin(); itr < devices.end(); itr++) {
        Device* device = *itr;
        uint8_t* data = new uint8_t[device->type->getNumberOfSlots()];
        for(int i = 0; i < device->type->getNumberOfSlots(); i++) {
            data[i] = 0;
        }
        step->addDeviceData((*itr), data);
    }

    return step;
}

std::map<std::string, Scene *> SceneRepository::getSceneMap() {
    return this->scenes;
}
