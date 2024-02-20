
/*
 * Generated by NK-CPP
 */
#include <spdlog/spdlog.h>
#include "IExternalDiagnostics.idl.hpp"
trafficlight::IExternalDiagnostics::IExternalDiagnostics(NkKosTransport *transport, nk_iid_t riid) : transport(transport), riid(riid) {
}
trafficlight::IExternalDiagnostics::IExternalDiagnostics(NkKosTransport *transport, const char *endpoint_name) : transport(transport) {
    riid = ServiceLocatorGetRiid(transport->handle, endpoint_name);
    if (INVALID_RIID == riid) {
        spdlog::error("Service RIID {} not found", endpoint_name);
    }
}
    nk_err_t trafficlight::IExternalDiagnostics::GetState(nk_uint8_t in_id) {
        trafficlight_IExternalDiagnostics_GetState_req req{
            {}, in_id
    };
    static trafficlight_IExternalDiagnostics_GetState_res res{};
    reqArena.reset();
    resArena.reset();
    nk_req_reset(&req);
    nk_req_reset(&res);
    nk_msg_set_method_id(
            &req,
            this->riid,
            trafficlight_IExternalDiagnostics_GetState_mid
    );
    nk_msg_set_method_id(
            &res,
            this->riid,
            trafficlight_IExternalDiagnostics_GetState_mid
    );
    nk_msg_set_ncaps(
            &req,
            trafficlight_IExternalDiagnostics_GetState_req_handles
    );
    nk_msg_set_ncaps(
            &res,
            trafficlight_IExternalDiagnostics_GetState_res_handles
    );
    
    nk_err_t rc = nk_transport_call(
            &this->transport->base,
            &req.base_,
            &reqArena,
            &res.base_,
            &resArena
    );
    return rc;
    }
    nk_err_t trafficlight::IExternalDiagnostics::GetErrors(nk_uint8_t in_id) {
        trafficlight_IExternalDiagnostics_GetErrors_req req{
            {}, in_id
    };
    static trafficlight_IExternalDiagnostics_GetErrors_res res{};
    reqArena.reset();
    resArena.reset();
    nk_req_reset(&req);
    nk_req_reset(&res);
    nk_msg_set_method_id(
            &req,
            this->riid,
            trafficlight_IExternalDiagnostics_GetErrors_mid
    );
    nk_msg_set_method_id(
            &res,
            this->riid,
            trafficlight_IExternalDiagnostics_GetErrors_mid
    );
    nk_msg_set_ncaps(
            &req,
            trafficlight_IExternalDiagnostics_GetErrors_req_handles
    );
    nk_msg_set_ncaps(
            &res,
            trafficlight_IExternalDiagnostics_GetErrors_res_handles
    );
    
    nk_err_t rc = nk_transport_call(
            &this->transport->base,
            &req.base_,
            &reqArena,
            &res.base_,
            &resArena
    );
    return rc;
    }