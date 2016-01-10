#include "Kernel.h"
#include "device/DeviceTypeRepository.h"
#include "easylogging++.h"
#include <thread>

Kernel::Kernel(Configuration* config) {
    this->config = config;
}

void Kernel::boot() {
    DeviceTypeRepository* deviceTypeRepository = new DeviceTypeRepository(this->config->deviceTypesPath);

    this->httpServer = new HttpServer(this->config->serverPort);
    this->httpServer->init();

    this->httpServer->start();
    //std::thread serverThread(&HttpServer::start, this->httpServer);
    //serverThread.join();
}
