#include "Kernel.h"
#include "device/DeviceTypeRepository.h"
#include "device/DeviceRepository.h"
#include "interface/InterfaceRepository.h"
#include "scene/SceneRepository.h"
#include "rendering/Renderer.h"
#include <thread>

Kernel::Kernel(Configuration* config) {
    this->config = config;
}

void Kernel::boot() {
    DeviceTypeRepository* deviceTypeRepository = new DeviceTypeRepository(this->config->deviceTypesPath);

    DeviceRepository* deviceRepository = new DeviceRepository(deviceTypeRepository);
    deviceRepository->loadFromFile(this->config->devicesPath);

    SceneRepository* sceneRepository = new SceneRepository(deviceRepository);
    sceneRepository->loadFromFile(this->config->scenesPath);

    InterfaceRepository* interfaceRepository = new InterfaceRepository();
    interfaceRepository->loadFromFile(this->config->interfacesPath);

    Renderer* renderer = new Renderer(interfaceRepository);
    renderer->init(deviceRepository);
    std::thread rendererThread(&Renderer::start, std::ref(renderer));

    this->httpServer = new HttpServer(this->config->serverPort);
    this->httpServer->init();
    this->httpServer->start();

    renderer->stop();
    rendererThread.join();
}

void Kernel::shutdown() {
    this->httpServer->stop();
}
