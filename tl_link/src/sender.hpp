#ifndef TRAFFICLIGHT_SENDER_H
#define TRAFFICLIGHT_SENDER_H

#include "client.hpp"
#include "IExternalControl.idl.hpp"


using namespace trafficlight;

class Connector {
private:
    ControlServerClient &api;
    IExternalControl &control;

    void push_state() {
        nk_uint8_t maintenance, manual;
        control.GetStatus(maintenance, manual);

        uint32_t modes[2];
        control.GetMode(0, modes[0]);
        control.GetMode(1, modes[1]);

        api.send_status(maintenance, manual, modes);
    }

    void push_errors() {
        std::string severity;
        IExternalControl::DirectionError err;

        auto rc = control.GetErrors(0, severity, err);
        if (rc != rcOk || severity == "OK") return; // no errors

        api.send_error(severity, err);
    }

    void pull_mode() {
        auto res = api.get_task();
        if (!res.has_value())
            return;

        auto type = res.value().at("type").get_string();
        auto contents = res.value().at("contents");

        L::info("New task from control server: {}", type);

        if (type == "program") {
            control.SetModeTable(
                    api.deserialize_program(contents)
            );
        } else if (type == "maintenance") {
            control.SetMaintenanceMode(contents.get_bool());
        } else if (type == "manual") {
            control.SetManualMode(contents.get_bool());
        } else if (type == "set") {
            control.EnterMode(
                    api.deserialize_mode(contents.at(0)),
                    api.deserialize_mode(contents.at(1))
            );
        }
    }

public:
    Connector(ControlServerClient &api, trafficlight::IExternalControl &control) : api(api), control(control) {
    }

    void run() {
        while (true) {
            push_state();
            push_errors();
            pull_mode();
            KosThreadSleep(200);
        }
    }
};

#endif