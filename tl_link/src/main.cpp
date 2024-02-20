#include "log.hpp"
#include <kos_net.h>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;

int main() {
    init_logging("TL|LN");

    if (!wait_for_network()) {
        L::info("Net  ERR!!");
        return 1;
    }

    L::info("External network connected!");

    return 0;
}
