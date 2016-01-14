#ifndef LIGHTER_HTTPSERVER_H
#define LIGHTER_HTTPSERVER_H

#include "../scene/SceneRepository.h"
#include "../rendering/Renderer.h"

class HttpServer {
public:
    HttpServer(int port, SceneRepository* sceneRepository, Renderer* renderer);
    void init();
    void start();
    void stop();

private:
    int port;
    int serverSocket;
    bool run = true;
    SceneRepository* sceneRepository;
    Renderer* renderer;
};


#endif //LIGHTER_HTTPSERVER_H
