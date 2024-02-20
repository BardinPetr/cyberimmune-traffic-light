#ifndef TRAFFIC_LIGHT_DIAGNOSTICSCONTROLLER_HPP
#define TRAFFIC_LIGHT_DIAGNOSTICSCONTROLLER_HPP

#include <random>
#include "trafficlight/Diagnostics.edl.h"
#include "log.hpp"
#include "XNkKosTransport.hpp"
#include "IDiagnostics.idl.hpp"
#include "mode.hpp"
#include "XNkKosTransportServer.hpp"
#include <fmt/ranges.h>

using namespace trafficlight;
using namespace std;

static const std::string SEVERITIES[]{
        "WARNING",
        "DANGER",
        "CRITICAL",
};

class DiagnosticsController : public trafficlight_ITargetState {
private:
    XNkKosTransport controlTransport;
    IDiagnostics *notifier;

    static nk_uint32_t rand_int() {
        return rand() % 100;
    }

    static vector<nk_uint32_t> getCurrents() {
        return {rand_int(), rand_int(), rand_int()};
    }

    static bool isError(bool requestedOn, nk_uint32_t current) {
        return requestedOn ^ (current > 20);
    }

public:
    explicit DiagnosticsController() : trafficlight_ITargetState() {
        static const trafficlight_ITargetState_ops impl_ops = {
                .NotifyCurrentState = NotifyCurrentState_impl
        };
        this->ops = &impl_ops;

        if (!controlTransport.connect("conn_control")) {
            L::error("Could not find connection conn_control");
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
        auto notifier = static_cast<DiagnosticsController *>(self)->notifier;

        vector<nk_uint32_t> currents = getCurrents();
        L::info("Current mode of ID{} is {}. Got currents for LED: {} (mA)",
                req->id, mode_to_string(req->mode), currents);

        notifier->NotifyState(req->id, currents);

        IDiagnostics::DirectionColor err{
                isError(req->mode & Red, currents[0]),
                isError(req->mode & Yellow, currents[1]),
                isError(req->mode & Green, currents[2])
        };

        int err_count = err.r + err.y + err.g;
        if (err_count > 0)
            notifier->NotifyFailure(SEVERITIES[err_count - 1], req->id, req->mode, err);

        return NK_EOK;
    }
};

#endif //TRAFFIC_LIGHT_DIAGNOSTICSCONTROLLER_HPP
