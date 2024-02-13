#include <coresrv/sl/sl_api.h>
#include <coresrv/nk/transport-kos.h>
#include "../../common/include/log.hpp"
#include "trafficlight/ILightMode.idl.h"
#include "ILightMode.idl.hpp"

using namespace trafficlight;

int main() {
    init_logging("TL|CT");

    Handle handle = ServiceLocatorConnect("conn_control_gpio");
    if (handle == INVALID_HANDLE) {
        L::error("Could not find connection conn_control_gpio");
        return 1;
    }

    NkKosTransport transport;
    NkKosTransport_Init(&transport, handle, NK_NULL, 0);

    ILightMode gpio(&transport, "lightModeGpio.mode");

    L::info("Control system started");

    ILightMode::CrossedDirectionsMode m{2, 4};
    gpio.SetMode(m);

    L::info("OOK");

    return 0;
}
