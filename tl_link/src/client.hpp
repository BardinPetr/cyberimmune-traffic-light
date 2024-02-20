#include <string>

#include "httpclient.hpp"


class ControlServerClient : public HTTPClient {

public:
    ControlServerClient(const std::string &host, int port) : HTTPClient(host, port) {}


};