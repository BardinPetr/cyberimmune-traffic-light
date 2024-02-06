#include <coresrv/sl/sl_api.h>
#include <coresrv/nk/transport-kos.h>
#include "trafficlight/GPIO.edl.h"
#include "trafficlight/ILightMode.idl.h"
#include "log.hpp"
#include "mode.h"

nk_err_t SetMode_impl(
        trafficlight_ILightMode *self,
        const trafficlight_ILightMode_SetMode_req *req,
        const nk_arena *reqArena,
        trafficlight_ILightMode_SetMode_res *res,
        nk_arena *resArena) {
    L::info("New SetMode request: {}  {}",
            mode_to_string(req->mode.dir0),
            mode_to_string(req->mode.dir1));
    return NK_EOK;
}

trafficlight_ILightMode *LightMode_impl() {
    static const trafficlight_ILightMode_ops ops = {
            .SetMode = SetMode_impl
    };
    static trafficlight_ILightMode impl{&ops};
    return &impl;
}

int main() {
    init_logging("TL|IO");

    ServiceId iid;
    Handle handle = ServiceLocatorRegister(
            "conn_control_gpio",
            nullptr, 0,
            &iid
    );

    NkKosTransport transport;
    NkKosTransport_Init(&transport, handle, RTL_NULL, 0);

    trafficlight_GPIO_component_req req{};
    char req_buffer[trafficlight_GPIO_component_req_arena_size];
    nk_arena req_arena = NK_ARENA_INITIALIZER(req_buffer, req_buffer + sizeof(req_buffer));

    trafficlight_GPIO_component_res res{};
    char res_buffer[trafficlight_GPIO_component_res_arena_size];
    nk_arena res_arena = NK_ARENA_INITIALIZER(res_buffer, res_buffer + sizeof(res_buffer));

    trafficlight_CLightMode_component mode_comp;
    trafficlight_CLightMode_component_init(&mode_comp, LightMode_impl());

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
