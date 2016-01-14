#include "easylogging++.h"
#include "configuration/Configuration.h"
#include "configuration/ConfigurationLoader.h"
#include "Kernel.h"
#include <csignal>

INITIALIZE_EASYLOGGINGPP

void configureLogging();
void signalHandler(int signal);

Kernel* kernel;

int main(int argv, char* argc[]) {
    START_EASYLOGGINGPP(argv, argc);
    configureLogging();

    char* configPath = (char *) "./config.json";
    LOG(INFO) << "Loading configuration from " << configPath;
    ConfigurationLoader* configLoader = new ConfigurationLoader();
    Configuration* config = configLoader->loadFromFile(configPath);
    VLOG(9) << "Configuration values:" << std::endl << config->toString();

    kernel = new Kernel(config);
    signal(SIGINT, signalHandler);
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

void signalHandler(int signal) {
    kernel->shutdown();
}