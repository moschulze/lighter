#include "ClientProcessor.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
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
    bool run = true;

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
}

void ClientProcessor::setClient(int client) {
    this->client = client;
}

HttpResponse *ClientProcessor::processApiRequest(HttpRequest* request) {
    std::string apiUri = request->uri.substr(4);
    if(apiUri.find("/scene/start/") == 0) {
        Scene* scene = this->sceneRepository->findById(apiUri.substr(13));
        if(scene == NULL) {
            return NULL;
        }

        this->renderer->startScene(scene);
        HttpResponse* response = new HttpResponse();
        response->body = "{\"success\":true}";
        return response;
    }

    return NULL;
}

HttpResponse *ClientProcessor::processFileRequest(HttpRequest* request) {
    return NULL;
}