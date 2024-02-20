
/*
 * Generated by NK-CPP
 */
#include <spdlog/spdlog.h>
#include "IExternalControl.idl.hpp"
trafficlight::IExternalControl::IExternalControl(NkKosTransport *transport, nk_iid_t riid) : transport(transport), riid(riid) {
}
trafficlight::IExternalControl::IExternalControl(NkKosTransport *transport, const char *endpoint_name) : transport(transport) {
    riid = ServiceLocatorGetRiid(transport->handle, endpoint_name);
    if (INVALID_RIID == riid) {
        spdlog::error("Service RIID {} not found", endpoint_name);
    }
}
    nk_err_t trafficlight::IExternalControl::SetMaintenanceMode(nk_uint8_t in_enable) {
        trafficlight_IExternalControl_SetMaintenanceMode_req req{
            {}, in_enable
    };
    static trafficlight_IExternalControl_SetMaintenanceMode_res res{};
    reqArena.reset();
    resArena.reset();
    nk_req_reset(&req);
    nk_req_reset(&res);
    nk_msg_set_method_id(
            &req,
            this->riid,
            trafficlight_IExternalControl_SetMaintenanceMode_mid
    );
    nk_msg_set_method_id(
            &res,
            this->riid,
            trafficlight_IExternalControl_SetMaintenanceMode_mid
    );
    nk_msg_set_ncaps(
            &req,
            trafficlight_IExternalControl_SetMaintenanceMode_req_handles
    );
    nk_msg_set_ncaps(
            &res,
            trafficlight_IExternalControl_SetMaintenanceMode_res_handles
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
    nk_err_t trafficlight::IExternalControl::SetManualMode(nk_uint8_t in_enable) {
        trafficlight_IExternalControl_SetManualMode_req req{
            {}, in_enable
    };
    static trafficlight_IExternalControl_SetManualMode_res res{};
    reqArena.reset();
    resArena.reset();
    nk_req_reset(&req);
    nk_req_reset(&res);
    nk_msg_set_method_id(
            &req,
            this->riid,
            trafficlight_IExternalControl_SetManualMode_mid
    );
    nk_msg_set_method_id(
            &res,
            this->riid,
            trafficlight_IExternalControl_SetManualMode_mid
    );
    nk_msg_set_ncaps(
            &req,
            trafficlight_IExternalControl_SetManualMode_req_handles
    );
    nk_msg_set_ncaps(
            &res,
            trafficlight_IExternalControl_SetManualMode_res_handles
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
    nk_err_t trafficlight::IExternalControl::EnterMode(nk_uint32_t in_dir1, nk_uint32_t in_dir2) {
        trafficlight_IExternalControl_EnterMode_req req{
            {}, in_dir1, in_dir2
    };
    static trafficlight_IExternalControl_EnterMode_res res{};
    reqArena.reset();
    resArena.reset();
    nk_req_reset(&req);
    nk_req_reset(&res);
    nk_msg_set_method_id(
            &req,
            this->riid,
            trafficlight_IExternalControl_EnterMode_mid
    );
    nk_msg_set_method_id(
            &res,
            this->riid,
            trafficlight_IExternalControl_EnterMode_mid
    );
    nk_msg_set_ncaps(
            &req,
            trafficlight_IExternalControl_EnterMode_req_handles
    );
    nk_msg_set_ncaps(
            &res,
            trafficlight_IExternalControl_EnterMode_res_handles
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
    nk_err_t trafficlight::IExternalControl::SetModeTable(const vector<ProgramStep>& in_prog) {
        trafficlight_IExternalControl_SetModeTable_req req{
            {}, {}
    };
    static trafficlight_IExternalControl_SetModeTable_res res{};
    reqArena.reset();
    resArena.reset();
    nk_req_reset(&req);
    nk_req_reset(&res);
    nk_msg_set_method_id(
            &req,
            this->riid,
            trafficlight_IExternalControl_SetModeTable_mid
    );
    nk_msg_set_method_id(
            &res,
            this->riid,
            trafficlight_IExternalControl_SetModeTable_mid
    );
    nk_msg_set_ncaps(
            &req,
            trafficlight_IExternalControl_SetModeTable_req_handles
    );
    nk_msg_set_ncaps(
            &res,
            trafficlight_IExternalControl_SetModeTable_res_handles
    );
    
        // begin arg req.prog IDLTypeList
    
        
    rtl_size_t cnt_in_prog_0 = in_prog.size();
    
        ProgramStep* ptr_in_prog_0 = nk_arena_alloc(
            ProgramStep,
            &reqArena,
            &req.prog,
            cnt_in_prog_0
        );
        rtl_memcpy(ptr_in_prog_0, (ProgramStep *) in_prog.data(), cnt_in_prog_0 * sizeof(ProgramStep));
    
    
    // end arg req.prog
    
    nk_err_t rc = nk_transport_call(
            &this->transport->base,
            &req.base_,
            &reqArena,
            &res.base_,
            &resArena
    );
    return rc;
    }
    nk_err_t trafficlight::IExternalControl::GetStatus() {
        trafficlight_IExternalControl_GetStatus_req req{
            {}, 
    };
    static trafficlight_IExternalControl_GetStatus_res res{};
    reqArena.reset();
    resArena.reset();
    nk_req_reset(&req);
    nk_req_reset(&res);
    nk_msg_set_method_id(
            &req,
            this->riid,
            trafficlight_IExternalControl_GetStatus_mid
    );
    nk_msg_set_method_id(
            &res,
            this->riid,
            trafficlight_IExternalControl_GetStatus_mid
    );
    nk_msg_set_ncaps(
            &req,
            trafficlight_IExternalControl_GetStatus_req_handles
    );
    nk_msg_set_ncaps(
            &res,
            trafficlight_IExternalControl_GetStatus_res_handles
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
    nk_err_t trafficlight::IExternalControl::GetMode(nk_uint8_t in_id) {
        trafficlight_IExternalControl_GetMode_req req{
            {}, in_id
    };
    static trafficlight_IExternalControl_GetMode_res res{};
    reqArena.reset();
    resArena.reset();
    nk_req_reset(&req);
    nk_req_reset(&res);
    nk_msg_set_method_id(
            &req,
            this->riid,
            trafficlight_IExternalControl_GetMode_mid
    );
    nk_msg_set_method_id(
            &res,
            this->riid,
            trafficlight_IExternalControl_GetMode_mid
    );
    nk_msg_set_ncaps(
            &req,
            trafficlight_IExternalControl_GetMode_req_handles
    );
    nk_msg_set_ncaps(
            &res,
            trafficlight_IExternalControl_GetMode_res_handles
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
    nk_err_t trafficlight::IExternalControl::GetErrors(nk_uint8_t in_id) {
        trafficlight_IExternalControl_GetErrors_req req{
            {}, in_id
    };
    static trafficlight_IExternalControl_GetErrors_res res{};
    reqArena.reset();
    resArena.reset();
    nk_req_reset(&req);
    nk_req_reset(&res);
    nk_msg_set_method_id(
            &req,
            this->riid,
            trafficlight_IExternalControl_GetErrors_mid
    );
    nk_msg_set_method_id(
            &res,
            this->riid,
            trafficlight_IExternalControl_GetErrors_mid
    );
    nk_msg_set_ncaps(
            &req,
            trafficlight_IExternalControl_GetErrors_req_handles
    );
    nk_msg_set_ncaps(
            &res,
            trafficlight_IExternalControl_GetErrors_res_handles
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
