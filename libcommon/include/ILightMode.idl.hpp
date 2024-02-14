
/*
* Generated by NK-CPP
*/
#ifndef TRAFFICLIGHT_ILIGHTMODE_IDL_HPP
#define TRAFFICLIGHT_ILIGHTMODE_IDL_HPP
#include <coresrv/sl/sl_api.h>
#include <coresrv/nk/transport-kos.h>
#include <vector>
#include "trafficlight/ILightMode.idl.h"
#include "XNkArena.hpp"

namespace trafficlight {
    using std::vector;
    class ILightMode {
    private:
        NkKosTransport *transport;
        nk_iid_t riid;
        XNkArena<trafficlight_ILightMode_req_arena_size> reqArena;
        XNkArena<trafficlight_ILightMode_res_arena_size> resArena;
    public:
    
        using ModeType = trafficlight_ILightMode_ModeType;
        using DirectionColor = trafficlight_ILightMode_DirectionColor;
        using DirectionMode = trafficlight_ILightMode_DirectionMode;
        using CrossedDirectionsMode = trafficlight_ILightMode_CrossedDirectionsMode;
    
        struct SetModeResponse {
            nk_err_t status;
        
        };
        ILightMode(NkKosTransport *transport, nk_iid_t riid);
        ILightMode(NkKosTransport *transport, const char* endpoint_name);
    
        const trafficlight_ILightMode_SetMode_res* SetMode(const CrossedDirectionsMode& mode);
    };
};
#endif //TRAFFICLIGHT_ILIGHTMODE_IDL_HPP
