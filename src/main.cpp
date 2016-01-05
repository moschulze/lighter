#include "easylogging++.h"
#include "configuration/Configuration.h"
#include "configuration/ConfigurationLoader.h"

INITIALIZE_EASYLOGGINGPP

int main(int argv, char* argc[]) {
    START_EASYLOGGINGPP(argv, argc);

    return 0;
}