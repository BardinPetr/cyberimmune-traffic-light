#ifndef TRAFFIC_LIGHT_XNKNS_H
#define TRAFFIC_LIGHT_XNKNS_H

#define NS_MAX_BUFFER 1000

#include <coresrv/sl/sl_api.h>
#include <coresrv/nk/transport-kos.h>
#include <coresrv/ns/ns_api.h>
#include <string>


struct NkServiceQualifier {
    bool valid;
    std::string server;
    std::string service;

    NkServiceQualifier() {}

    NkServiceQualifier(const std::string &server, const std::string &service) : valid(true), server(server),
                                                                                service(service) {}
};

class XNkNameService {
private:
    NsHandle handle;


public:
    XNkNameService(const std::string &ns_name = nullptr, nk_uint32_t connect_timeout_ms = 10000) {
        auto rc = NsCreate(ns_name.c_str(), connect_timeout_ms, &handle);
        if (rc != rcOk) {
            throw rc; // TODO: create execeptions
        }
    }

    NkServiceQualifier getService(const std::string &interface_name, unsigned int index) {
        NkServiceQualifier out{};
        auto rc = NsEnumServices(
                handle,
                interface_name.c_str(), index,
                out.server.data(), NS_MAX_BUFFER,
                out.service.data(), NS_MAX_BUFFER
        );
        if (rc == rcResourceNotFound) {
//            return ;
        }
        if (rc == rcOk) {

        }
        throw rc;
    }

};

#endif //TRAFFIC_LIGHT_XNKNS_H
