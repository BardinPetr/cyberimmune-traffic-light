#include <kos/thread.h>
#include "log.hpp"
#include "client.hpp"
#include "IExternalControl.idl.hpp"
#include "XNkKosTransport.hpp"
#include "mode.hpp"

using namespace std;
using namespace trafficlight;

int main() {
    init_logging("TL|LN");

    if (!wait_for_network()) {
        L::critical("Failed to set up networking");
        return 1;
    }
    L::info("External network connected!");


    ControlServerClient client("10.0.2.2", 5000);

    XNkKosTransport transport;
    if (!transport.connect("conn_control")) {
        L::error("Could not find connection conn_control");
        exit(1);
    }
    IExternalControl source(&transport, "externalInput.ctrl");

    return 0;
}