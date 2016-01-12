#include "HttpResponse.h"
#include <sstream>

void HttpResponse::setHeader(std::string key, std::string value) {
    this->headers[key] = value;
}

std::string HttpResponse::toString() {
    std::stringstream ret;

    this->setHeader("Content-Length", std::to_string(this->body.length()));

    ret << "HTTP/" << this->httpVersion << " " << this->statusCode << " " << this->getStatusMessage(this->statusCode) << "\r\n";
    for(std::map<std::string, std::string>::iterator itr = this->headers.begin(); itr != this->headers.end(); itr++) {
        ret << itr->first << ": " << itr->second << "\r\n";
    }
    ret << "\r\n" << this->body;

    return ret.str();
}

std::string HttpResponse::getStatusMessage(int code) {
    switch(code) {
        case 200:
            return "OK";
        case 301:
            return "Moved Permanently";
        case 302:
            return "Found";
        case 400:
            return "Bad Request";
        case 401:
            return "Unauthorized";
        case 403:
            return "Forbidden";
        case 404:
            return "Not Found";
        case 405:
            return "Method Not Allowed";
        case 500:
            return "Internal Server Error";
    }

    return "";
}
