#ifndef TRAFFICLIGHT_CLIENT_H
#define TRAFFICLIGHT_CLIENT_H

#include <string>

#include "httpclient.hpp"
#include "mode.hpp"

using namespace trafficlight;

class ControlServerClient : HTTPClient {
private:
    int uid;

    json::value serialize_mode(uint32_t mode) {
        return {
                {"r",     (bool) (mode & Red)},
                {"y",     (bool) (mode & Yellow)},
                {"g",     (bool) (mode & Green)},
                {"blink", (bool) (mode & Blink)},
        };
    }

public:
    ControlServerClient(int uid, const std::string &host, int port) : HTTPClient(host, port), uid(uid) {}

    void send_status(bool is_maintenance, bool is_manual, uint32_t modes[2]) {
        this->do_post("/status", {
                {"id", uid},
                {"is_maintenance", is_maintenance},
                {"is_manual", is_manual},
                {"modes", {serialize_mode(modes[0]), serialize_mode(modes[1])}}
        });
    }
};


#endif