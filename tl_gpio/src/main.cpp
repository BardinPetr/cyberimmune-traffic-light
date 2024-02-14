#include "trafficlight/GPIO.edl.h"
#include "log.hpp"
#include "mode.hpp"
#include "XNkKosTransport.hpp"
#include "ITargetState.idl.hpp"
#include "LightModeController.hpp"
#include "XNkKosTransportServer.hpp"

using namespace trafficlight;
using namespace std;

using EntityGPIOServer = XNK_SERVER(trafficlight_GPIO_entity);


int main() {
    init_logging("TL|IO");

    LightModeController ctrl;
    trafficlight_CLightMode_component mode_comp;
    trafficlight_CLightMode_component_init(&mode_comp, &ctrl);

    trafficlight_GPIO_entity entity;
    trafficlight_GPIO_entity_init(&entity, &mode_comp);

    EntityGPIOServer transport(&entity);
    if (!transport.serve("conn_control_gpio")) {
        L::error("Failed to register conn_control_gpio");
        exit(1);
    }

    L::info("Server running...");
    transport.pool();
    return 0;
}
