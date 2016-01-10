#include "Kernel.h"
#include "device/DeviceTypeRepository.h"
#include "device/DeviceRepository.h"
#include "interface/InterfaceRepository.h"
#include <thread>

Kernel::Kernel(Configuration* config) {
    this->config = config;
}

void Kernel::boot() {
    DeviceTypeRepository* deviceTypeRepository = new DeviceTypeRepository(this->config->deviceTypesPath);

    DeviceRepository* deviceRepository = new DeviceRepository(deviceTypeRepository);
    deviceRepository->loadFromFile(this->config->devicesPath);

    InterfaceRepository* interfaceRepository = new InterfaceRepository();
    interfaceRepository->loadFromFile(this->config->interfacesPath);

    this->httpServer = new HttpServer(this->config->serverPort);
    this->httpServer->init();

    this->httpServer->start();
    //std::thread serverThread(&HttpServer::start, this->httpServer);
    //serverThread.join();
}
