#include "log.hpp"
#include "StateController.hpp"
#include "DiagnosticsReceiver.hpp"
#include "XNkKosTransport.hpp"
#include "XNkKosTransportServer.hpp"

using EntityControlServer = XNK_SERVER(trafficlight_Control_entity);

int main() {
    init_logging("TL|CT");

    // Launch mode sender
    pthread_t ctrl_tid;
    pthread_attr_t t_attr;
    pthread_attr_init(&t_attr);
    pthread_create(&ctrl_tid, &t_attr, start_controller, nullptr);

    // Launch diagnostics receiver
    DiagnosticsReceiver receiver;
    trafficlight_CDiagnostics_component mode_comp;
    trafficlight_CDiagnostics_component_init(&mode_comp, &receiver);

    trafficlight_Control_entity entity;
    trafficlight_Control_entity_init(&entity, &mode_comp);

    EntityControlServer transport(&entity);
    if (!transport.serve("conn_diagnostics_control")) {
        L::error("Failed to register conn_control_gpio");
        exit(1);
    }

    L::info("Server running...");
    transport.pool();

    pthread_join(ctrl_tid, nullptr);
    return 0;
}
