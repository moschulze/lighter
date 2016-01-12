#include "ClientProcessor.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include <string>
#include <sys/socket.h>
#include <unistd.h>

void ClientProcessor::process() {
    char buffer[INPUT_BUFFER_SIZE];
    int bytesRead = 1;
    std::string input;
    bool run = true;

    while(bytesRead > 0) {
        bytesRead = recv(this->client, buffer, (INPUT_BUFFER_SIZE-1), MSG_NOSIGNAL);
        if(bytesRead > 0) {
            for(int i = 0; i < bytesRead;i++) {
                input += buffer[i];
            }
            if(input.compare(input.size()-5, 4, "\r\n\r\n")) {
                break;
            }
        }
    }

    HttpRequest* request = new HttpRequest();
    request->buildFromString(input);

    HttpResponse* response = new HttpResponse();
    response->body = "Lighter";
    response->setHeader("Server", "Lighter");
    std::string responseString = response->toString();
    write(this->client, responseString.c_str(), responseString.length());
}

void ClientProcessor::setClient(int client) {
    this->client = client;
}
