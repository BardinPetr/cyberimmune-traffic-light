#include "trafficlight/Diagnostics.edl.h"
#include "log.hpp"
#include "XNkKosTransport.hpp"
#include "IDiagnostics.idl.hpp"
#include "mode.hpp"

using namespace trafficlight;

class Diagnostics : public trafficlight_ITargetState {
private:
    XNkKosTransport controlTransport;
    IDiagnostics *notifier;

public:
    explicit Diagnostics() : trafficlight_ITargetState() {
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

    ~Diagnostics() {
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
        auto notifier = static_cast<Diagnostics *>(self)->notifier;

        notifier->NotifyState(0, v);

        if (true) {
            vector<nk_uint8_t> v2;
            IDiagnostics::DirectionColor err{1, 0, 1};
            notifier->NotifyFailure(v2, 0, req->mode, err);
        }

        return NK_EOK;
    }
};

int main() {
    init_logging("TL|DI");

    XNkKosTransport gpio_in_transport;
    if (!gpio_in_transport.serve("conn_gpio_diagnostics")) {
        L::error("Failed to register conn_gpio_diagnostics");
        exit(1);
    }
    trafficlight_Diagnostics_component_req req{};
    char req_buffer[trafficlight_Diagnostics_component_req_arena_size];
    nk_arena req_arena = NK_ARENA_INITIALIZER(req_buffer, req_buffer + sizeof(req_buffer));

    trafficlight_Diagnostics_component_res res{};
    char res_buffer[trafficlight_Diagnostics_component_res_arena_size];
    nk_arena res_arena = NK_ARENA_INITIALIZER(res_buffer, res_buffer + sizeof(res_buffer));

    Diagnostics d;
    trafficlight_CTargetState_component inputComponent;
    trafficlight_CTargetState_component_init(&inputComponent, &d);

    trafficlight_Diagnostics_entity entity;
    trafficlight_Diagnostics_entity_init(&entity, &inputComponent);

    L::info("Server running...");

    while (true) {
        nk_req_reset(&req);
        nk_arena_reset(&req_arena);
        nk_arena_reset(&res_arena);

        auto status = nk_transport_recv(&gpio_in_transport.base, &req.base_, &req_arena);
        if (status != NK_EOK) continue;
        L::error("IR {}", status);

        status = trafficlight_Diagnostics_entity_dispatch(
                &entity,
                &req.base_, &req_arena,
                &res.base_, &res_arena
        );
        if (status != NK_EOK) {
            L::error("Dispatch error: {}", status);
            continue;
        }

        status = nk_transport_reply(&gpio_in_transport.base, &res.base_, &res_arena);
        if (status != NK_EOK)
            L::error("Send error: {}", status);
    }

    return 0;
}
