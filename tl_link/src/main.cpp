#include "log.hpp"
#include "client.hpp"

using namespace std;


int main() {
    init_logging("TL|LN");

    if (!wait_for_network()) {
        L::critical("Failed to set up networking");
        return 1;
    }
    L::info("External network connected!");


    ControlServerClient client("10.0.2.2", 5000);
    cerr << client.do_get("/").value_or("ERR") << "\n";
    cerr << client.do_get("/q").value_or("ERR") << "\n";
    cerr << client.do_post("/q", "tewerwerwe").value_or("ERR") << "\n";

    return 0;
}
