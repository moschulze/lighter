#ifndef LIGHTER_HTTPREQUEST_H
#define LIGHTER_HTTPREQUEST_H


#include <string>
#include <map>

class HttpRequest {
public:
    void buildFromString(std::string input);
    std::string toString();

private:
    std::string method = "";
    std::string uri = "";
    std::string httpVersion = "";
    std::map<std::string, std::string> headers;
    std::string body = "";
};


#endif //LIGHTER_HTTPREQUEST_H
