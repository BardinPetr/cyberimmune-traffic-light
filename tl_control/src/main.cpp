#include "log.hpp"
#include "StateController.hpp"
#include "DiagnosticsReceiver.hpp"
#include "XNkKosTransport.hpp"
#include "XNkKosTransportServer.hpp"
#include "Logger.hpp"
#include <unistd.h>


using EntityControlServer = XNK_SERVER(trafficlight_Control_entity);

StateController controller;

void *start_controller(void *ctx) {
    controller.start();
    return nullptr;
}

int main() {
    init_logging("TL|CT");

    Logger logger;

    // Launch mode sender
    pthread_t ctrl_tid;
    pthread_attr_t t_attr;
    pthread_attr_init(&t_attr);
    pthread_create(&ctrl_tid, &t_attr, start_controller, nullptr);

    // Launch diagnostics receiver
    DiagnosticsReceiver receiver(controller, logger);
    trafficlight_CDiagnostics_component mode_comp;
    trafficlight_CDiagnostics_component_init(&mode_comp, &receiver);

    trafficlight_CExternalControl_component ext_comp;
    trafficlight_CExternalControl_component_init(&ext_comp, &controller);

    trafficlight_Control_entity entity;
    trafficlight_Control_entity_init(&entity, &mode_comp, &ext_comp);

    EntityControlServer transport(&entity);
    if (!transport.serve("conn_control")) {
        L::error("Failed to register conn_control");
        exit(1);
    }

    L::info("Server running...");
    transport.pool();

    pthread_join(ctrl_tid, nullptr);
    return 0;
}
