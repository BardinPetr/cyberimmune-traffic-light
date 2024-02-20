
/*
* Generated by NK-CPP
*/
#ifndef TRAFFICLIGHT_IEXTERNALDIAGNOSTICS_IDL_HPP
#define TRAFFICLIGHT_IEXTERNALDIAGNOSTICS_IDL_HPP
#include <coresrv/sl/sl_api.h>
#include <coresrv/nk/transport-kos.h>
#include <rtl/string.h>
#include <string>
#include <vector>
#include "trafficlight/IExternalDiagnostics.idl.h"
#include "XNkArena.hpp"
namespace trafficlight {
    using std::vector;
    class IExternalDiagnostics {
    private:
        NkKosTransport *transport;
        nk_iid_t riid;
        XNkArena<trafficlight_IExternalDiagnostics_req_arena_size> reqArena;
        XNkArena<trafficlight_IExternalDiagnostics_res_arena_size> resArena;
    public:
    
        using ModeType = trafficlight_IExternalDiagnostics_ModeType;
        using DirectionError = trafficlight_IExternalDiagnostics_DirectionError;
    
        struct GetStateResponse {
            nk_err_t status;
        
            nk_uint32_t mode;
        };
        struct GetErrorsResponse {
            nk_err_t status;
        
            std::string severity;
            DirectionError errors;
        };
        IExternalDiagnostics(NkKosTransport *transport, nk_iid_t riid);
        IExternalDiagnostics(NkKosTransport *transport, const char* endpoint_name);
    
        nk_err_t GetState(nk_uint8_t in_id);
        nk_err_t GetErrors(nk_uint8_t in_id);
    };
};
#endif //TRAFFICLIGHT_IEXTERNALDIAGNOSTICS_IDL_HPP
