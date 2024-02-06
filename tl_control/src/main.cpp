#include <coresrv/sl/sl_api.h>
#include <coresrv/nk/transport-kos.h>
#include <kos/thread.h>
#include "trafficlight/Control.edl.h"
#include "trafficlight/ILightMode.idl.h"
#include "log.hpp"

trafficlight_ILightMode_proxy proxy;

bool setMode(uint32_t mode0, uint32_t mode1) {
    trafficlight_ILightMode_SetMode_req req{
            {},
            {mode0, mode1}
    };
    trafficlight_ILightMode_SetMode_res res;

    auto status = trafficlight_ILightMode_SetMode(&proxy.base, &req, nullptr, &res, nullptr);
    if (status == NK_EOK) {
        L::info("setMode {:05b} {:05b} OK", mode0, mode1);
        return true;
    }
    L::debug("setMode {:05b} {:05b} failed with code: {}", mode0, mode1, status);
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

    L::info("Control system started");

    KosThreadSleep(1000);
    for (uint16_t m0 = 0; m0 < 32; ++m0) {
        for (uint16_t m1 = 0; m1 < 32; ++m1) {
            setMode(m0, m1);
        }
    }
    return 0;
}
