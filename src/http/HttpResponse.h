#ifndef LIGHTER_HTTPRESPONSE_H
#define LIGHTER_HTTPRESPONSE_H

#include <string>
#include <map>

class HttpResponse {
public:
    HttpResponse();
    std::string body = "";
    int statusCode = 200;
    std::string httpVersion = "1.1";
    void setHeader(std::string key, std::string value);
    std::string toString();

private:
    std::map<std::string, std::string> headers;
    std::string getStatusMessage(int code);
};


#endif //LIGHTER_HTTPRESPONSE_H
