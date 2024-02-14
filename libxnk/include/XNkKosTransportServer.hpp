#ifndef TRAFFIC_LIGHT_XNKKOSTRANSPORTSERVER_HPP
#define TRAFFIC_LIGHT_XNKKOSTRANSPORTSERVER_HPP

#define XNK_SERVER(entity) \
    XNkKosTransportServer<\
            entity,\
            entity##_req,\
            entity##_res,\
            entity##_req_arena_size,\
            entity##_res_arena_size,\
            entity##_dispatch>

#include <coresrv/sl/sl_api.h>
#include <coresrv/nk/transport-kos.h>
#include <string>

#include "XNkKosTransport.hpp"


template<class Entity,
        class Request,
        class Response,
        int req_arena_size,
        int res_arena_size,
        auto dispatcher>
class XNkKosTransportServer : public XNkKosTransport {
private:
    Entity *entity;
    Request req{};
    Response res{};
    XNkArena<req_arena_size> req_arena{};
    XNkArena<res_arena_size> res_arena{};

public:
    explicit XNkKosTransportServer(Entity *entity) : XNkKosTransport(), entity(entity) {
    }

    void pool() {
        while (true) {
            nk_req_reset(&req);
            nk_arena_reset(&req_arena);
            nk_arena_reset(&res_arena);

            auto status = nk_transport_recv(&this->base, &req.base_, &req_arena);
            if (status != NK_EOK)
                continue;

            status = dispatcher(
                    entity,
                    &req.base_, &req_arena,
                    &res.base_, &res_arena
            );
            if (status != NK_EOK)
                continue;

            nk_transport_reply(&this->base, &res.base_, &res_arena);
        }
    }
};

#endif //TRAFFIC_LIGHT_XNKKOSTRANSPORTSERVER_HPP
