#ifndef TRAFFIC_LIGHT_DIAGNOSTICSCONTROLLER_H
#define TRAFFIC_LIGHT_DIAGNOSTICSCONTROLLER_H

#include "trafficlight/Diagnostics.edl.h"
#include "log.hpp"
#include "XNkKosTransport.hpp"
#include "IDiagnostics.idl.hpp"
#include "mode.hpp"
#include "XNkKosTransportServer.hpp"

using namespace trafficlight;


class DiagnosticsController : public trafficlight_ITargetState {
private:
    XNkKosTransport controlTransport;
    IDiagnostics *notifier;

public:
    explicit DiagnosticsController() : trafficlight_ITargetState() {
        static const trafficlight_ITargetState_ops impl_ops = {
                .NotifyCurrentState = NotifyCurrentState_impl
        };
        this->ops = &impl_ops;

        if (!controlTransport.connect("conn_diagnostics_control")) {
            L::error("Could not find connection conn_diagnostics_control");
            exit(1);
        }
        notifier = new IDiagnostics(&controlTransport, "diagnosticsInput.diag");
    }

    ~DiagnosticsController() {
        delete notifier;
    }

    static nk_err_t NotifyCurrentState_impl(
            trafficlight_ITargetState *self,
            const trafficlight_ITargetState_NotifyCurrentState_req *req,
            const nk_arena *reqArena,
            trafficlight_ITargetState_NotifyCurrentState_res *res,
            nk_arena *resArena) {
        L::info("Current mode of ID{} is {}", req->id, mode_to_string(req->mode));

        vector<nk_uint32_t> v;
        auto notifier = static_cast<DiagnosticsController *>(self)->notifier;

        notifier->NotifyState(0, v);

        if (true) {
            vector<nk_uint8_t> v2;
            IDiagnostics::DirectionColor err{1, 0, 1};
            notifier->NotifyFailure(v2, 0, req->mode, err);
        }

        return NK_EOK;
    }
};

#endif //TRAFFIC_LIGHT_DIAGNOSTICSCONTROLLER_H
