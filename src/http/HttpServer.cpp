#include "HttpServer.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <thread>
#include "easylogging++.h"
#include "ClientProcessor.h"


HttpServer::HttpServer(int port) {
    this->port = port;
}

void HttpServer::init() {
    this->serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if(this->serverSocket == -1) {
        LOG(FATAL) << "Cant't create socket";
    }

    int yes = 1;
    setsockopt(this->serverSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

    struct sockaddr_in sockAddr;
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    sockAddr.sin_port = htons(this->port);
    if(bind(this->serverSocket, (struct sockaddr *)&sockAddr, sizeof(sockAddr)) < 0) {
        LOG(FATAL) << "Unable to bind to port " << this->port;
    }
    listen(this->serverSocket, 5);
}

void HttpServer::start() {
    while(this->run) {
        int client = accept(this->serverSocket, NULL, NULL);
        ClientProcessor* clientProcessor = new ClientProcessor();
        clientProcessor->setClient(client);
        clientProcessor->process();

        close(client);
    }

    close(this->serverSocket);
}

void HttpServer::stop() {
    this->run = false;
}
