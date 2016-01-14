#ifndef LIGHTER_CLIENTPROCESSOR_H
#define LIGHTER_CLIENTPROCESSOR_H

#include "HttpResponse.h"
#include "HttpRequest.h"

#define INPUT_BUFFER_SIZE 1024


class ClientProcessor {
public:
    void process();
    void setClient(int client);

private:
    int client;
    HttpResponse* processApiRequest(HttpRequest* request);
};


#endif //LIGHTER_CLIENTPROCESSOR_H
