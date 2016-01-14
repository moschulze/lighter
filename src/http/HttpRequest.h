#ifndef LIGHTER_HTTPREQUEST_H
#define LIGHTER_HTTPREQUEST_H


#include <string>
#include <map>

class HttpRequest {
public:
    void buildFromString(std::string input);
    std::string toString();
    std::string method = "";
    std::string uri = "";
    std::string httpVersion = "";
    std::string body = "";

private:
    std::map<std::string, std::string> headers;
};


#endif //LIGHTER_HTTPREQUEST_H
