#ifndef LIGHTER_HTTPSERVER_H
#define LIGHTER_HTTPSERVER_H


class HttpServer {
public:
    HttpServer(int port);
    void init();
    void start();
    void stop();

private:
    int port;
    int serverSocket;
    bool run = true;
};


#endif //LIGHTER_HTTPSERVER_H
