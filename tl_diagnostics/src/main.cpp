#include "trafficlight/Diagnostics.edl.h"
#include "IDiagnostics.idl.hpp"
#include "XNkKosTransportServer.hpp"
#include "DiagnosticsController.h"
#include "log.hpp"

using EntityDiagnosticServer = XNK_SERVER(trafficlight_Diagnostics_entity);


int main() {
    init_logging("TL|DI");

    DiagnosticsController d;
    trafficlight_CTargetState_component inputComponent;
    trafficlight_CTargetState_component_init(&inputComponent, &d);

    trafficlight_Diagnostics_entity entity;
    trafficlight_Diagnostics_entity_init(&entity, &inputComponent);

    EntityDiagnosticServer transport(&entity);
    if (!transport.serve("conn_gpio_diagnostics")) {
        L::error("Failed to register conn_gpio_diagnostics");
        exit(1);
    }

    L::info("Server running...");
    transport.pool();
    return 0;
}
