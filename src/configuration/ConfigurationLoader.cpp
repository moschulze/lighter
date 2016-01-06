#include "ConfigurationLoader.h"
#include "rapidjson/filereadstream.h"

#include "rapidjson/document.h"

Configuration* ConfigurationLoader::loadFromFile(char *path) {
    FILE* filePointer = fopen(path, "r");
    char buffer[65536];
    rapidjson::FileReadStream inputStream(filePointer, buffer, sizeof(buffer));

    rapidjson::Document document;
    document.ParseStream(inputStream);

    fclose(filePointer);

    Configuration* config = new Configuration();
    for(rapidjson::Value::ConstMemberIterator itr = document.MemberBegin(); itr != document.MemberEnd(); ++itr) {
        if(strcmp(itr->name.GetString(), "server.port") == 0) {
            config->serverPort = itr->value.GetInt();
        } else if(strcmp(itr->name.GetString(), "data.path") == 0) {
            config->dataPath = itr->value.GetString();
        }
    }

    return config;
}