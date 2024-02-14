#include <kos/thread.h>
#include "trafficlight/Control.edl.h"
#include "ILightMode.idl.hpp"
#include "XNkKosTransport.hpp"
#include "log.hpp"

using namespace trafficlight;

class StateController {
private:
    XNkKosTransport transport;
    ILightMode *gpio;
public:
    StateController() {
        if (!transport.connect("conn_control_gpio")) {
            L::error("Could not find connection conn_control_gpio");
            exit(1);
        }
        gpio = new ILightMode(&transport, "lightModeGpio.mode");
    }

    ~StateController() {
        delete gpio;
    }

    void start() {
        L::info("Control thread started");

        ILightMode::CrossedDirectionsMode m{2, 4};
        while (true) {
            gpio->SetMode(m);
            KosThreadSleep(1000);
        }
    }
};


void *start_controller(void *ctx) {
    StateController controller;
    controller.start();
    return nullptr;
}