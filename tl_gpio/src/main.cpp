#include "trafficlight/GPIO.edl.h"
#include "log.hpp"
#include "mode.hpp"
#include "XNkKosTransport.hpp"
#include "ITargetState.idl.hpp"

using namespace trafficlight;
using namespace std;

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

int main() {
    init_logging("TL|IO");

    XNkKosTransport transport;
    transport.serve("conn_control_gpio");
    if (!transport.serve("conn_control_gpio")) {
        L::error("Failed to register conn_control_gpio");
        exit(1);
    }
    trafficlight_GPIO_component_req req{};
    char req_buffer[trafficlight_GPIO_component_req_arena_size];
    nk_arena req_arena = NK_ARENA_INITIALIZER(req_buffer, req_buffer + sizeof(req_buffer));

    trafficlight_GPIO_component_res res{};
    char res_buffer[trafficlight_GPIO_component_res_arena_size];
    nk_arena res_arena = NK_ARENA_INITIALIZER(res_buffer, res_buffer + sizeof(res_buffer));

    LightModeController ctrl;
    trafficlight_CLightMode_component mode_comp;
    trafficlight_CLightMode_component_init(&mode_comp, &ctrl);

    trafficlight_GPIO_entity entity;
    trafficlight_GPIO_entity_init(&entity, &mode_comp);

    L::info("Server running...");

    while (true) {
        nk_req_reset(&req);
        nk_arena_reset(&req_arena);
        nk_arena_reset(&res_arena);

        auto status = nk_transport_recv(&transport.base, &req.base_, &req_arena);
        if (status != NK_EOK) continue;

        status = trafficlight_GPIO_entity_dispatch(
                &entity,
                &req.base_, &req_arena,
                &res.base_, &res_arena
        );
        if (status != NK_EOK) {
            L::error("Dispatch error: {}", status);
            continue;
        }

        status = nk_transport_reply(&transport.base, &res.base_, &res_arena);
        if (status != NK_EOK)
            L::error("Send error: {}", status);
    }

    return 0;
}
