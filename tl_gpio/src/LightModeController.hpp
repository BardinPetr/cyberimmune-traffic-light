#ifndef TRAFFIC_LIGHT_LIGHTMODECONTROLLER_H
#define TRAFFIC_LIGHT_LIGHTMODECONTROLLER_H

using namespace trafficlight;


class LightModeController : public trafficlight_ILightMode {
private:
    XNkKosTransport diagnosticsTransport;
    ITargetState *diagnosticsNotifier;

public:
    explicit LightModeController() : trafficlight_ILightMode() {
        static const trafficlight_ILightMode_ops impl_ops = {
                .SetMode = SetMode_impl
        };
        this->ops = &impl_ops;


        if (!diagnosticsTransport.connect("conn_gpio_diagnostics")) {
            L::error("Could not find connection conn_gpio_diagnostics");
            exit(1);
        }
        diagnosticsNotifier = new ITargetState(&diagnosticsTransport, "stateInput.stateInput");
    }

    ~LightModeController() {
        delete diagnosticsNotifier;
    }

    static nk_err_t SetMode_impl(
            trafficlight_ILightMode *self,
            const trafficlight_ILightMode_SetMode_req *req,
            const nk_arena *reqArena,
            trafficlight_ILightMode_SetMode_res *res,
            nk_arena *resArena) {

        L::info("New SetMode request: {}  {}",
                mode_to_string(req->mode.dir0),
                mode_to_string(req->mode.dir1));

        auto notifier = static_cast<LightModeController *>(self)->diagnosticsNotifier;
        notifier->NotifyCurrentState(0, req->mode.dir0);
        notifier->NotifyCurrentState(1, req->mode.dir1);

        return NK_EOK;
    }
};

#endif //TRAFFIC_LIGHT_LIGHTMODECONTROLLER_H
