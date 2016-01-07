#ifndef LIGHTER_KERNEL_H
#define LIGHTER_KERNEL_H

#include "configuration/Configuration.h"
#include "http/HttpServer.h"

class Kernel {
public:
    Kernel(Configuration* config);
    void boot();
private:
    Configuration* config;
    HttpServer* httpServer;
};


#endif //LIGHTER_KERNEL_H
