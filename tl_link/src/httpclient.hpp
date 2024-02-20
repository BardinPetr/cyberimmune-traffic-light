#ifndef TRAFFICLIGHT_HTTPCLIENT_H
#define TRAFFICLIGHT_HTTPCLIENT_H

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/json.hpp>
#include <kos_net.h>
#include <iostream>
#include <exception>
#include <string>
#include "jsonbody.hpp"

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
namespace json = boost::json;
using tcp = net::ip::tcp;

class HTTPClient {

private:
    net::io_context ctx;
    beast::tcp_stream stream;

    const std::string &host;
    tcp::resolver::results_type endpoint;

public:

    HTTPClient(const std::string &host, int port) : host(host), stream(ctx) {
        tcp::resolver resolver(ctx);
        endpoint = resolver.resolve(host, std::to_string(port));
    }

    template<class Body>
    std::optional<http::response<json_body>> do_request(const http::request<Body> &req) {
        http::response<json_body> res;
        beast::flat_buffer buffer;

        try {
            stream.connect(endpoint);
            http::write(stream, req);
            http::read(stream, buffer, res);

            beast::error_code ec;
            stream.socket().shutdown(tcp::socket::shutdown_both, ec);
            if (ec && ec != beast::errc::not_connected)
                return std::nullopt;
        }
        catch (const std::exception &e) {
            L::error("Network error: {}", e.what());
            return std::nullopt;
        }

        if (res.base().result_int() != 200) {
            L::error("HTTP error: {}", res.base().reason().data());
            return std::nullopt;
        }

        return res;
    }

    template<class Body>
    http::request<Body> create_request(http::verb method, const std::string &target) {
        http::request<Body> req{method, target, 11};
        req.set(http::field::host, host);
        return req;
    }

    std::optional<json::value> do_get(const std::string &target) {
        auto req = create_request<http::empty_body>(http::verb::get, target);

        auto res = do_request(req);

        if (res.has_value())
            return res.value().body();
        return std::nullopt;
    }

    std::optional<json::value> do_post(const std::string &target, const json::value &body) {
        auto req = create_request<json_body>(http::verb::post, target);
        req.set(http::field::content_type, "application/json");
        req.body() = body;
        req.prepare_payload();

        auto res = do_request(req);

        if (res.has_value())
            return res.value().body();
        return std::nullopt;
    }
};

#endif // TRAFFICLIGHT_HTTPCLIENT_H
