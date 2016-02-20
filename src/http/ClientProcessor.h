#ifndef LIGHTER_CLIENTPROCESSOR_H
#define LIGHTER_CLIENTPROCESSOR_H

#include "HttpResponse.h"
#include "HttpRequest.h"
#include "../scene/SceneRepository.h"
#include "../rendering/Renderer.h"

#define INPUT_BUFFER_SIZE 1024


class ClientProcessor {
public:
    ClientProcessor(SceneRepository* sceneRepository, Renderer* renderer);
    void process();
    void setClient(int client);

private:
    int client;
    HttpResponse* processApiRequest(HttpRequest* request);
    HttpResponse* processFileRequest(HttpRequest* request);
    SceneRepository* sceneRepository;
    Renderer* renderer;
    HttpResponse* startScene(std::string sceneId);
    HttpResponse* stopScene(std::string sceneId);
    HttpResponse* listScenes();
};


#endif //LIGHTER_CLIENTPROCESSOR_H
