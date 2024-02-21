#ifndef TRAFFICLIGHT_SENDER_H
#define TRAFFICLIGHT_SENDER_H

#include "client.hpp"
#include "IExternalControl.idl.hpp"

class Connector {
private:
    ControlServerClient &api;
    trafficlight::IExternalControl &control;

    void push_state() {
        nk_uint8_t maintenance, manual;
        control.GetStatus(maintenance, manual);

        uint32_t modes[2];
        control.GetMode(0, modes[0]);
        control.GetMode(1, modes[1]);

        api.send_status(maintenance, manual, modes);
    }

public:
    Connector(ControlServerClient &api, trafficlight::IExternalControl &control) : api(api), control(control) {
    }

    void run() {
        while (true) {
            push_state();
            KosThreadSleep(1000);
        }
    }
};

#endif