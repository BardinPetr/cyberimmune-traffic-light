#include <kos/thread.h>
#include "trafficlight/Control.edl.h"
#include "ILightMode.idl.hpp"
#include "XNkKosTransport.hpp"
#include "mode.hpp"
#include "log.hpp"

using namespace trafficlight;
using CDM = ILightMode::CrossedDirectionsMode;

const CDM MODE_SEQ[]{
        CDM{Green, Red},
        CDM{Green | Blink, Red},
        CDM{Yellow, Red},
        CDM{Red, Red},
        CDM{Red, Red | Yellow},
        CDM{Red, Green},
        CDM{Red, Green | Blink},
        CDM{Red, Yellow},
        CDM{Red, Red},
        CDM{Red | Yellow, Red},
};
const size_t MODE_SEQ_SIZE = sizeof(MODE_SEQ) / sizeof(CDM);

class StateController {
private:
    XNkKosTransport transport;
    ILightMode *gpio;

    size_t curMode = 0;

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

        while (true) {
            gpio->SetMode(MODE_SEQ[curMode]);
            curMode = (curMode + 1) % MODE_SEQ_SIZE;
            KosThreadSleep(2000);
        }
    }
};


void *start_controller(void *ctx) {
    StateController controller;
    controller.start();
    return nullptr;
}