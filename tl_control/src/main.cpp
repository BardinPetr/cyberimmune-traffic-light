#include "log.hpp"
#include "StateController.hpp"
#include "DiagnosticsReceiver.hpp"
#include "XNkKosTransport.hpp"

int main() {
    init_logging("TL|CT");

    pthread_t ctrl_tid;
    pthread_attr_t t_attr;
    pthread_attr_init(&t_attr);
    pthread_create(&ctrl_tid, &t_attr, start_controller, nullptr);


    DiagnosticsReceiver receiver;

    XNkKosTransport transport;
    if (!transport.serve("conn_diagnostics_control")) {
        L::error("Failed to register conn_control_gpio");
        exit(1);
    }

    trafficlight_Control_component_req req{};
    char req_buffer[trafficlight_Control_component_req_arena_size];
    nk_arena req_arena = NK_ARENA_INITIALIZER(req_buffer, req_buffer + sizeof(req_buffer));

    trafficlight_Control_component_res res{};
    char res_buffer[trafficlight_Control_component_res_arena_size];
    nk_arena res_arena = NK_ARENA_INITIALIZER(res_buffer, res_buffer + sizeof(res_buffer));

    trafficlight_CDiagnostics_component mode_comp;
    trafficlight_CDiagnostics_component_init(&mode_comp, &receiver);

    trafficlight_Control_entity entity;
    trafficlight_Control_entity_init(&entity, &mode_comp);

    L::info("Server running...");

    while (true) {
        nk_req_reset(&req);
        nk_arena_reset(&req_arena);
        nk_arena_reset(&res_arena);

        auto status = nk_transport_recv(&transport.base, &req.base_, &req_arena);
        if (status != NK_EOK) continue;

        status = trafficlight_Control_entity_dispatch(
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

    pthread_join(ctrl_tid, nullptr);
    return 0;
}
