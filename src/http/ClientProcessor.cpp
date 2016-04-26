#include "ClientProcessor.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "HttpJsonResponse.h"
#include <string>
#include <sys/socket.h>
#include <unistd.h>

ClientProcessor::ClientProcessor(SceneRepository *sceneRepository, Renderer* renderer) {
    this->sceneRepository = sceneRepository;
    this->renderer = renderer;
}

void ClientProcessor::process() {
    char buffer[INPUT_BUFFER_SIZE];
    int bytesRead = 1;
    std::string input;

    while(bytesRead > 0) {
        bytesRead = recv(this->client, buffer, (INPUT_BUFFER_SIZE-1), MSG_NOSIGNAL);
        if(bytesRead > 0) {
            for(int i = 0; i < bytesRead;i++) {
                input += buffer[i];
            }
            if(input.compare(input.size()-5, 4, "\r\n\r\n")) {
                break;
            }
        }
    }

    HttpRequest* request = new HttpRequest();
    request->buildFromString(input);

    HttpResponse* response = NULL;
    if(request->uri.find("/api/") == 0) {
        response = this->processApiRequest(request);
    } else {
        response = this->processFileRequest(request);
    }

    if(response == NULL) {
        response = new HttpResponse();
        response->statusCode = 404;
        response->body = "The requested resource was not found";
    }

    std::string responseString = response->toString();
    write(this->client, responseString.c_str(), responseString.length());

    delete response;
}

void ClientProcessor::setClient(int client) {
    this->client = client;
}

HttpResponse *ClientProcessor::processApiRequest(HttpRequest* request) {
    std::string apiUri = request->uri.substr(4);
    if(apiUri.find("/scene/start/") == 0) {
        return this->startScene(apiUri.substr(13));
    }

    if(apiUri.find("/scene/stop/") == 0) {
        return this->stopScene(apiUri.substr(12));
    }

    if(apiUri.find("/scene/list/") == 0) {
        return this->listScenes();
    }

    return NULL;
}

HttpResponse *ClientProcessor::startScene(std::string sceneId) {
    Scene* scene = this->sceneRepository->findById(sceneId);
    if(scene == NULL) {
        return NULL;
    }

    this->renderer->startScene(scene);
    HttpResponse* response = new HttpJsonResponse();
    response->body = "{\"success\":true}";
    return response;
}

HttpResponse *ClientProcessor::stopScene(std::string sceneId) {
    Scene* scene = this->sceneRepository->findById(sceneId);
    if(scene == NULL) {
        return NULL;
    }

    this->renderer->stopScene(scene);
    HttpResponse* response = new HttpJsonResponse();
    response->body = "{\"success\":true}";
    return response;
}

HttpResponse *ClientProcessor::listScenes() {
    std::map<std::string, Scene*> scenes = this->sceneRepository->getSceneMap();
    std::string responseBody = "{";
    auto itr = scenes.begin();
    while(itr != scenes.end()) {
        responseBody.append("\"");
        responseBody.append(itr->second->id);
        responseBody.append("\":{\"name\":\"");
        responseBody.append(itr->second->name);
        responseBody.append("\",\"active\":");
        if(itr->second->activeStep.compare("") == 0) {
            responseBody.append("false");
        } else {
            responseBody.append("true");
        }
        responseBody.append("}");

        ++itr;
        if(itr != scenes.end()) {
            responseBody.append(",");
        }
    }
    responseBody.append("}");

    HttpResponse* response = new HttpJsonResponse();
    response->body = responseBody;
    return response;
}

HttpResponse *ClientProcessor::processFileRequest(HttpRequest* request) {
    if(request->uri.length() == 0) {
        return NULL;
    }

    if(request->uri.compare("/") == 0) {
        request->uri = "/index.html";
    }

    std::string filePath = "webroot/";
    filePath.append(request->uri.substr(1).c_str());

    FILE *file = fopen(filePath.c_str(), "r");
    if(!file) {
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);
    char *buffer = (char*) malloc(sizeof(char) * fileSize + 1);
    fread(buffer, 1, fileSize, file);
    fclose(file);

    buffer[fileSize] = '\0';
    HttpResponse *response = new HttpResponse();
    response->body = buffer;

    return response;
}