#include "HttpRequest.h"
#include "easylogging++.h"
#include <sstream>

void HttpRequest::buildFromString(std::string input) {
    int position = 0;

    //Method
    while(position < input.length() && input[position] != ' ') {
        this->method += input[position];
        position++;
    }
    position++;

    //URI
    while(position < input.length() && input[position] != ' ') {
        this->uri += input[position];
        position++;
    }
    position += 6;

    //Http version
    while(position < input.length() && input[position] != '\r') {
        this->httpVersion += input[position];
        position++;
    }

    //Jump to headers
    while(position < input.length() && (input[position] == '\r' || input[position] == '\n')) {
        position++;
    }

    //Headers
    while(position < input.length()) {
        std::string key = "";
        std::string value = "";

        //Key
        while(position < input.length() && input[position] != ':') {
            key += input[position];
            position++;
        }

        position += 2;

        //Value
        while(position < input.length() && input[position] != '\r') {
            value += input[position];
            position++;
        }

        this->headers[key] = value;

        if(input.substr((unsigned long) position, 4).compare("\r\n\r\n") == 0) {
            break;
        }

        position += 2;
    }

    position += 4;

    //Body
    while(position < input.length()) {
        this->body += input[position];
        position++;
    }
}

std::string HttpRequest::toString() {
    std::stringstream ret;

    ret << this->method << " " << this->uri << " HTTP/" << this->httpVersion << "\r\n";
    for(std::map<std::string, std::string>::iterator it = this->headers.begin(); it != this->headers.end(); ++it) {
        ret << it->first << ": " << it->second << "\r\n";
    }
    ret << "\r\n";
    ret << this->body << "\r\n\r\n";

    return ret.str();
}
