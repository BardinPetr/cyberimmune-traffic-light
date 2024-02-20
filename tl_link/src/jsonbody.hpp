#ifndef TRAFFICLIGHT_JSONBODY_H
#define TRAFFICLIGHT_JSONBODY_H

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/json.hpp>
#include <boost/json/stream_parser.hpp>
#include <boost/json/monotonic_resource.hpp>
#include <boost/asio/buffer.hpp>

namespace json = boost::json;
namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;

struct json_body {
    using value_type = json::value;

    struct writer {
        using const_buffers_type = net::const_buffer;

        template<bool isRequest, class Fields>
        writer(http::header<isRequest, Fields> const &h, value_type const &body) {
            serializer.reset(&body);
        }

        void init(boost::system::error_code &ec) {
            ec = {};
        }

        boost::optional<std::pair<const_buffers_type, bool>> get(boost::system::error_code &ec) {
            ec = {};
            const auto len = serializer.read(buffer, sizeof(buffer));
            return std::make_pair(
                    net::const_buffer(len.data(), len.size()),
                    !serializer.done()
            );
        }

    private:
        json::serializer serializer;
        char buffer[32768];
    };

    struct reader {
        template<bool isRequest, class Fields>
        reader(http::header<isRequest, Fields> &h, value_type &body) : body(body) {
        }

        void init(boost::optional<std::uint64_t> const &content_length, boost::system::error_code &ec) {
            if (content_length)
                parser.reset(json::make_shared_resource<json::monotonic_resource>(*content_length));
            ec = {};
        }

        template<class ConstBufferSequence>
        std::size_t put(ConstBufferSequence const &buffers, boost::system::error_code &ec) {
            ec = {};
            return parser.write_some(static_cast<const char *>(buffers.data()), buffers.size(), ec);
        }

        void finish(boost::system::error_code &ec) {
            ec = {};
            if (parser.done())
                body = parser.release();
            else
                ec = boost::json::error::incomplete;
        }

    private:
        json::stream_parser parser;
        value_type &body;
    };
};

#endif //TRAFFICLIGHT_JSONBODY_H
