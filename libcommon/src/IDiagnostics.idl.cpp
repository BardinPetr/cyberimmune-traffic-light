
/*
 * Generated by NK-CPP
 */
#include <spdlog/spdlog.h>
#include <rtl/string.h>
#include "IDiagnostics.idl.hpp"
#include <stdexcept>

trafficlight::IDiagnostics::IDiagnostics(NkKosTransport *transport, nk_iid_t riid) : transport(transport), riid(riid) {
}

trafficlight::IDiagnostics::IDiagnostics(NkKosTransport *transport, const char *endpoint_name) : transport(transport) {
    riid = ServiceLocatorGetRiid(transport->handle, endpoint_name);
    if (INVALID_RIID == riid) {
        spdlog::error("Service RIID {} not found", endpoint_name);
    }
}

const trafficlight_IDiagnostics_NotifyFailure_res *
trafficlight::IDiagnostics::NotifyFailure(const std::string &severity,
                                          nk_uint8_t id,
                                          nk_uint32_t requested,
                                          const DirectionColor &problem) {
    trafficlight_IDiagnostics_NotifyFailure_req req{
            {}, {}, id, requested, problem
    };
    static trafficlight_IDiagnostics_NotifyFailure_res res{};

    reqArena.reset();
    resArena.reset();
    nk_req_reset(&req);
    nk_req_reset(&res);
    nk_msg_set_method_id(
            &req,
            this->riid,
            trafficlight_IDiagnostics_NotifyFailure_mid
    );
    nk_msg_set_method_id(
            &res,
            this->riid,
            trafficlight_IDiagnostics_NotifyFailure_mid
    );
    nk_msg_set_ncaps(
            &req,
            trafficlight_IDiagnostics_NotifyFailure_req_handles
    );
    nk_msg_set_ncaps(
            &res,
            trafficlight_IDiagnostics_NotifyFailure_res_handles
    );

    rtl_size_t sz = severity.length() + 1;
    nk_char_t *ptr = nk_arena_alloc(
            nk_char_t,
            &reqArena,
            &req.severity,
            sz
    );
    rtl_strncpy((char *) ptr, (const char *) severity.c_str(), sz);

    nk_err_t rc = nk_transport_call(
            &this->transport->base,
            &req.base_,
            &reqArena,
            &res.base_,
            &resArena
    );
    if (rc != rcOk) {
        spdlog::error("s ne {}", rc);
        throw std::runtime_error("Sending error");
    }

    return &res;
}

const trafficlight_IDiagnostics_NotifyState_res *
trafficlight::IDiagnostics::NotifyState(nk_uint8_t id, const vector<nk_uint32_t> &measured) {
    trafficlight_IDiagnostics_NotifyState_req req{
            {}, id, {}
    };
    static trafficlight_IDiagnostics_NotifyState_res res{};

    reqArena.reset();
    resArena.reset();
    nk_req_reset(&req);
    nk_req_reset(&res);
    nk_msg_set_method_id(
            &req,
            this->riid,
            trafficlight_IDiagnostics_NotifyState_mid
    );
    nk_msg_set_method_id(
            &res,
            this->riid,
            trafficlight_IDiagnostics_NotifyState_mid
    );
    nk_msg_set_ncaps(
            &req,
            trafficlight_IDiagnostics_NotifyState_req_handles
    );
    nk_msg_set_ncaps(
            &res,
            trafficlight_IDiagnostics_NotifyState_res_handles
    );


    rtl_size_t sz = measured.size();
    nk_uint32_t *ptr = nk_arena_alloc(
            nk_uint32_t,
            &reqArena,
            &req.measured,
            sz
    );
    rtl_memcpy(ptr, (nk_uint32_t *) measured.data(), sz * sizeof(nk_uint32_t));

    nk_err_t rc = nk_transport_call(
            &this->transport->base,
            &req.base_,
            &reqArena,
            &res.base_,
            &resArena
    );

    return &res;
}
