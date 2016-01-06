#include "easylogging++.h"
#include "configuration/Configuration.h"
#include "configuration/ConfigurationLoader.h"
#include "Kernel.h"

INITIALIZE_EASYLOGGINGPP

void configureLogging();

int main(int argv, char* argc[]) {
    START_EASYLOGGINGPP(argv, argc);
    configureLogging();

    char* configPath = (char *) "./config.json";
    LOG(INFO) << "Loading configuration from " << configPath;
    ConfigurationLoader* configLoader = new ConfigurationLoader();
    Configuration* config = configLoader->loadFromFile(configPath);
    VLOG(9) << "Configuration values:" << std::endl << config->toString();

    Kernel* kernel = new Kernel(config);
    kernel->boot();

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