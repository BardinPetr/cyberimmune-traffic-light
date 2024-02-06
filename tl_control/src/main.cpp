#include <coresrv/sl/sl_api.h>
#include <coresrv/nk/transport-kos.h>
#include <kos/thread.h>
#include "trafficlight/GPIO.edl.h"
#include "trafficlight/ILightMode.idl.h"
#include "log.hpp"

trafficlight_ILightMode_proxy proxy;

bool setMode(const uint8_t modes[2]) {
    L::info("Calling setMode");

    trafficlight_ILightMode_SetMode_req req;
    memcpy(req.mode, modes, 2);

    trafficlight_ILightMode_SetMode_res res;
    auto status = trafficlight_ILightMode_SetMode(&proxy.base, &req, nullptr, &res, nullptr);
    if (status == NK_EOK) {
        L::info("setMode OK");
        return true;
    }
    L::info("setMode failed with code: {}", status);
    return false;
}

int main() {
    init_logging("TL|CT");

    Handle handle = ServiceLocatorConnect("conn_control_gpio");
    if (handle == INVALID_HANDLE) {
        L::error("Could not find connection conn_control_gpio");
        return 1;
    }

    NkKosTransport transport;
    NkKosTransport_Init(&transport, handle, NK_NULL, 0);

    nk_iid_t riid = ServiceLocatorGetRiid(handle, "lightModeGpio.mode");
    if (INVALID_RIID == riid) {
        L::error("Service RIID not found");
        return 1;
    }

    trafficlight_ILightMode_proxy_init(&proxy, &transport.base, riid);

    uint8_t i = 0;
    while (true) {
        setMode(new uint8_t[2]{i, i++});
        KosThreadSleep(1000);
    }

    return 0;
}
