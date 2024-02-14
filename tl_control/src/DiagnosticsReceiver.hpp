#include "trafficlight/Control.edl.h"
#include "mode.hpp"


class DiagnosticsReceiver : public trafficlight_IDiagnostics {
private:
public:
    DiagnosticsReceiver() : trafficlight_IDiagnostics() {
        static const trafficlight_IDiagnostics_ops impl_ops = {
                .NotifyFailure=NotifyFailure_impl,
                .NotifyState=NotifyState_impl
        };
        this->ops = &impl_ops;
    }

    static nk_err_t NotifyState_impl(
            trafficlight_IDiagnostics *self,
            const trafficlight_IDiagnostics_NotifyState_req *req,
            const nk_arena *reqArena,
            trafficlight_IDiagnostics_NotifyState_res *res,
            nk_arena *resArena) {
        L::info("LED Currents of ID{}", req->id);

//        L::info("LED Currents of ID{} is R:{}, Y:{}, G:{}",
//                req->id,
//                req->measured[0], req->measured[1], req->measured[2]);
        return NK_EOK;
    }

    static nk_err_t NotifyFailure_impl(
            trafficlight_IDiagnostics *self,
            const trafficlight_IDiagnostics_NotifyFailure_req *req,
            const nk_arena *reqArena,
            trafficlight_IDiagnostics_NotifyFailure_res *res,
            nk_arena *resArena) {

        L::error("{} failure of ID{} for mode {} got problems: R:{}, Y:{}, G:{}",
                "",
                req->id,
                mode_to_string(req->requested),
                req->problem.r, req->problem.y, req->problem.g);

        return NK_EOK;
    }
};
