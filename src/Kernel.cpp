#include "Kernel.h"
#include <thread>

Kernel::Kernel(Configuration* config) {
    this->config = config;
}

void Kernel::boot() {
    this->httpServer = new HttpServer(this->config->serverPort);
    this->httpServer->init();

    std::thread serverThread(&HttpServer::start, this->httpServer);
    serverThread.join();
}
