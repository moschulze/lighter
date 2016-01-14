#include "HttpJsonResponse.h"

HttpJsonResponse::HttpJsonResponse() : HttpResponse() {
    this->setHeader("Content-Type", "application/json");
}
