#ifndef LIGHTER_KERNEL_H
#define LIGHTER_KERNEL_H

#include "configuration/Configuration.h"

class Kernel {
public:
    Kernel(Configuration* config);
    void boot();
private:
    Configuration* config;
};


#endif //LIGHTER_KERNEL_H
