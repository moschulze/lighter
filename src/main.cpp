#include "easylogging++.h"

INITIALIZE_EASYLOGGINGPP

void configureLogging();

int main(int argv, char* argc[]) {
    START_EASYLOGGINGPP(argv, argc);
    configureLogging();

    return 0;
}

void configureLogging() {
    el::Configurations defaultConf;
    defaultConf.setToDefault();
    defaultConf.set(
            el::Level::Global,
            el::ConfigurationType::Format,
            "%datetime{%d.%M.%Y %H:%m:%s} [%level]: %msg"
    );
    el::Loggers::reconfigureLogger("default", defaultConf);
}