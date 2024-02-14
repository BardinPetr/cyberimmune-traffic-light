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
        nk_uint32_t cnt;
        nk_uint32_t *severity = nk_arena_get(nk_uint32_t, reqArena, &req->measured, &cnt);

        if (cnt != 3)
            return NK_EINVAL;

        L::info("LED Currents of ID{} is R:{}mA, Y:{}mA, G:{}mA",
                req->id,
                severity[0], severity[1], severity[2]);

        return NK_EOK;
    }

    static nk_err_t NotifyFailure_impl(
            trafficlight_IDiagnostics *self,
            const trafficlight_IDiagnostics_NotifyFailure_req *req,
            const nk_arena *reqArena,
            trafficlight_IDiagnostics_NotifyFailure_res *res,
            nk_arena *resArena) {
        nk_uint32_t str_len;
        nk_char_t *severity = nk_arena_get(nk_char_t, reqArena, &req->severity, &str_len);

        L::error("[{}] failure of ID{} for mode {} got problems: R:{}, Y:{}, G:{}",
                 severity,
                 req->id,
                 mode_to_string(req->requested),
                 req->problem.r, req->problem.y, req->problem.g);

        return NK_EOK;
    }
};
