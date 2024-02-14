#ifndef TRAFFIC_LIGHT_XNKKOSTRANSPORT_HPP
#define TRAFFIC_LIGHT_XNKKOSTRANSPORT_HPP

#include <coresrv/sl/sl_api.h>
#include <coresrv/nk/transport-kos.h>

class XNkKosTransport : public NkKosTransport {
public:
    XNkKosTransport() : NkKosTransport() {
    }

    bool connect(Handle ipc_handle) {
        NkKosTransport_Init(this, ipc_handle, NK_NULL, 0);
        return true;
    }

    bool connect(const char *channel_name) {
        Handle h = ServiceLocatorConnect(channel_name);
        if (h == INVALID_HANDLE) return false;
        return connect(h);
    }

    bool serve(const char *channel_name) {
        ServiceId iid;
        Handle h = ServiceLocatorRegister(channel_name, nullptr, 0, &iid);
        if (h == INVALID_HANDLE) return false;
        return connect(h);
    }
};

#endif //TRAFFIC_LIGHT_XNKKOSTRANSPORT_HPP
