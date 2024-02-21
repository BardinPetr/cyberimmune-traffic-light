#ifndef TRAFFICLIGHT_CLIENT_H
#define TRAFFICLIGHT_CLIENT_H

#include <string>

#include "httpclient.hpp"
#include "mode.hpp"
#include "IExternalControl.idl.hpp"

using namespace trafficlight;

class ControlServerClient : HTTPClient {
private:
    int uid;

    static json::value serialize_mode(uint32_t mode) {
        return {
                {"r",     (bool) (mode & Red)},
                {"y",     (bool) (mode & Yellow)},
                {"g",     (bool) (mode & Green)},
                {"blink", (bool) (mode & Blink)},
        };
    }

    static json::value serialize_error(IExternalControl::DirectionError error) {
        return {error.r, error.y, error.g};
    }

public:
    ControlServerClient(int uid, const std::string &host, int port) : HTTPClient(host, port), uid(uid) {}

    void send_status(bool is_maintenance, bool is_manual, uint32_t modes[2]) {
        this->do_post("/status", {
                {"id",             uid},
                {"is_maintenance", is_maintenance},
                {"is_manual",      is_manual},
                {"modes",          {serialize_mode(modes[0]), serialize_mode(modes[1])}}
        });
    }

    void send_error(const std::string &severity, IExternalControl::DirectionError err) {
        this->do_post("/error", {
                {"id",       uid},
                {"severity", severity},
                {"error",    serialize_error(err)}
        });
    }

    std::optional<json::value> get_task() {
        return this->do_get("/remote/" + std::to_string(uid));
    }

    static nk_uint32_t deserialize_mode(const json::value &val) {
        return (val.at("r").get_bool() << 0) |
               (val.at("y").get_bool() << 1) |
               (val.at("g").get_bool() << 2) |
               (val.at("blink").get_bool() << 4);
    }

    static vector<IExternalControl::ProgramStep> deserialize_program(const json::value &val) {
        vector<IExternalControl::ProgramStep> res;
        for (auto i: val.as_array()) {
            res.push_back(
                    {
                            {
                                    deserialize_mode(i.at("mode").at(0)),
                                    deserialize_mode(i.at("mode").at(1))
                            },
                            static_cast<nk_uint64_t>(i.at("time").get_int64())
                    }
            );
        }

        return res;
    }
};


#endif