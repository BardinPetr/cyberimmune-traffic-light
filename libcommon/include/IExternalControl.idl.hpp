
/*
* Generated by NK-CPP
*/
#ifndef TRAFFICLIGHT_IEXTERNALCONTROL_IDL_HPP
#define TRAFFICLIGHT_IEXTERNALCONTROL_IDL_HPP
#include <coresrv/sl/sl_api.h>
#include <coresrv/nk/transport-kos.h>
#include <rtl/string.h>
#include <string>
#include <vector>
#include "trafficlight/IExternalControl.idl.h"
#include "XNkArena.hpp"
namespace trafficlight {
    using std::vector;
    class IExternalControl {
    private:
        NkKosTransport *transport;
        nk_iid_t riid;
        XNkArena<trafficlight_IExternalControl_req_arena_size> reqArena;
        XNkArena<trafficlight_IExternalControl_res_arena_size> resArena;
    public:
    
        using ModeType = trafficlight_IExternalControl_ModeType;
        using CrossedDirectionsMode = trafficlight_IExternalControl_CrossedDirectionsMode;
        using ProgramStep = trafficlight_IExternalControl_ProgramStep;
        using DirectionError = trafficlight_IExternalControl_DirectionError;
        using Program = trafficlight_IExternalControl_Program;
        IExternalControl(NkKosTransport *transport, nk_iid_t riid);
        IExternalControl(NkKosTransport *transport, const char* endpoint_name);
    
        nk_err_t SetMaintenanceMode(nk_uint8_t in_enable);
        nk_err_t SetManualMode(nk_uint8_t in_enable);
        nk_err_t EnterMode(nk_uint32_t in_dir1, nk_uint32_t in_dir2);
        nk_err_t SetModeTable(const vector<ProgramStep>& in_prog);
        nk_err_t GetStatus(nk_uint8_t& out_maintenance, nk_uint8_t& out_manual);
        nk_err_t GetMode(nk_uint8_t in_id, nk_uint32_t& out_mode);
        nk_err_t GetErrors(nk_uint8_t in_id, std::string& out_severity, DirectionError& out_errors);
    };
};
#endif //TRAFFICLIGHT_IEXTERNALCONTROL_IDL_HPP
