#include "HttpServer.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <thread>
#include <fcntl.h>
#include <netdb.h>
#include "easylogging++.h"
#include "ClientProcessor.h"


HttpServer::HttpServer(int port, SceneRepository* sceneRepository, Renderer* renderer) {
    this->port = port;
    this->sceneRepository = sceneRepository;
    this->renderer = renderer;
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

        ClientProcessor* clientProcessor = new ClientProcessor(this->sceneRepository, this->renderer);
        clientProcessor->setClient(client);
        clientProcessor->process();

        close(client);
    }

    close(this->serverSocket);
}

void HttpServer::stop() {
    this->run = false;

    //dummy connection to terminate server
    struct sockaddr_in serverAddress;
    struct hostent *server;
    int client = socket(AF_INET, SOCK_STREAM, 0);
    server = gethostbyname("localhost");
    bzero((char*) &serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    bcopy((char*) server->h_addr, (char*) &serverAddress.sin_addr.s_addr, server->h_length);
    serverAddress.sin_port = htons(this->port);
    connect(client, (struct sockaddr *) &serverAddress, sizeof(serverAddress));
    close(client);
}
