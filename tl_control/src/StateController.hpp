#include <kos/thread.h>
#include <kos/mutex.h>
#include <queue>
#include "trafficlight/Control.edl.h"
#include "IExternalControl.idl.hpp"
#include "ILightMode.idl.hpp"
#include "XNkKosTransport.hpp"
#include "mode.hpp"
#include "log.hpp"

using namespace trafficlight;
using CDM = IExternalControl::CrossedDirectionsMode;
using ProgramStep = IExternalControl::ProgramStep;


struct Failure {
    std::string severity;
    IExternalControl::DirectionError err;
};

class StateController : public trafficlight_IExternalControl {
private:
    XNkKosTransport transport;
    ILightMode *gpio;

    size_t curProgramStep = 0;
    bool isMaintenance = false;
    bool isManual = false;
    CDM manualMode{};
    CDM currentMode{};

    KosMutex errorQueueMutex{};
    KosMutex programMutex{};

    std::queue<Failure> failures[2] = {};
    vector<ProgramStep> modeProgram{
            {{Green, Red},   1000},
            {{Red,   Green}, 1000},
    };

    void applyMode(CDM mode) {
        currentMode = mode;
        gpio->SetMode({mode.dir0, mode.dir1});
    }

    static nk_err_t SetMaintenanceMode(
            trafficlight_IExternalControl *self,
            const trafficlight_IExternalControl_SetMaintenanceMode_req *req,
            const nk_arena *reqArena,
            trafficlight_IExternalControl_SetMaintenanceMode_res *res,
            nk_arena *resArena) {
        auto s = static_cast<StateController *>(self);
        L::warn("Maintenance mode set to {}", req->enable);
        s->isMaintenance = req->enable;
        return rcOk;
    }

    static nk_err_t SetManualMode(
            trafficlight_IExternalControl *self,
            const trafficlight_IExternalControl_SetManualMode_req *req,
            const nk_arena *reqArena,
            trafficlight_IExternalControl_SetManualMode_res *res,
            nk_arena *resArena) {
        static_cast<StateController *>(self)->isManual = req->enable;
        L::warn("Manual mode set to {}", req->enable);
        return rcOk;
    }

    static nk_err_t EnterMode(
            trafficlight_IExternalControl *self,
            const trafficlight_IExternalControl_EnterMode_req *req,
            const nk_arena *reqArena,
            trafficlight_IExternalControl_EnterMode_res *res,
            nk_arena *resArena) {
        auto s = static_cast<StateController *>(self);
        s->manualMode.dir0 = req->dir1;
        s->manualMode.dir1 = req->dir2;
        L::warn("Requested manual mode {} {}", req->dir1, req->dir2);
        return rcOk;
    }

    static nk_err_t SetModeTable(
            trafficlight_IExternalControl *self,
            const trafficlight_IExternalControl_SetModeTable_req *req,
            const nk_arena *reqArena,
            trafficlight_IExternalControl_SetModeTable_res *res,
            nk_arena *resArena) {
        auto s = static_cast<StateController *>(self);

        nk_uint32_t steps;
        ProgramStep *prog = nk_arena_get(ProgramStep, reqArena, &req->prog, &steps);

        L::warn("Requested new program for {} steps", steps);

        if (steps == 0)
            return rcOk;

        KosMutexLock(&s->programMutex);
        s->modeProgram.clear();
        s->modeProgram.insert(s->modeProgram.cbegin(), prog, prog + steps);
        s->curProgramStep = 0;
        KosMutexUnlock(&s->programMutex);
        return rcOk;
    }

    static nk_err_t GetStatus(
            trafficlight_IExternalControl *self,
            const trafficlight_IExternalControl_GetStatus_req *req,
            const nk_arena *reqArena,
            trafficlight_IExternalControl_GetStatus_res *res,
            nk_arena *resArena) {
        auto s = static_cast<StateController *>(self);
        res->maintenance = s->isMaintenance;
        res->manual = s->isManual && !s->isMaintenance;
        return rcOk;
    }

    static nk_err_t GetMode(
            trafficlight_IExternalControl *self,
            const trafficlight_IExternalControl_GetMode_req *req,
            const nk_arena *reqArena,
            trafficlight_IExternalControl_GetMode_res *res,
            nk_arena *resArena) {
        auto m = static_cast<StateController *>(self)->currentMode;
        res->mode = req->id == 0 ? m.dir0 : m.dir1;
        return rcOk;
    }

    static nk_err_t GetErrors(
            trafficlight_IExternalControl *self,
            const trafficlight_IExternalControl_GetErrors_req *req,
            const nk_arena *reqArena,
            trafficlight_IExternalControl_GetErrors_res *res,
            nk_arena *resArena) {
        auto s = static_cast<StateController *>(self);

        auto &failures = s->failures[req->id];
        std::string severity("OK");
        if (!failures.empty()) {
            KosMutexLock(&s->errorQueueMutex);
            severity = failures.front().severity;
            res->errors = failures.front().err;
            failures.pop();
            KosMutexUnlock(&s->errorQueueMutex);
        }

        rtl_size_t len = severity.length() + 1;
        nk_char_t *ptr = nk_arena_alloc(
                nk_char_t,
                resArena,
                &res->severity,
                len
        );
        rtl_strncpy((char *) ptr, (const char *) severity.c_str(), len);
        return rcOk;
    }

public:
    StateController() : trafficlight_IExternalControl() {
        static const trafficlight_IExternalControl_ops impl_ops = {
                .SetMaintenanceMode=SetMaintenanceMode,
                .SetManualMode=SetManualMode,
                .EnterMode=EnterMode,
                .SetModeTable=SetModeTable,
                .GetStatus=GetStatus,
                .GetMode=GetMode,
                .GetErrors=GetErrors
        };
        this->ops = &impl_ops;

        if (!transport.connect("conn_control_gpio")) {
            L::error("Could not find connection conn_control_gpio");
            exit(1);
        }
        gpio = new ILightMode(&transport, "lightModeGpio.mode");

        KosMutexInit(&errorQueueMutex);
        KosMutexInit(&programMutex);
    }

    ~StateController() {
        delete gpio;
    }

    void start() {
        L::info("Control thread started");

        while (true) {
            if (isMaintenance) {
                applyMode({Yellow | Blink, Yellow | Blink});
            } else if (isManual) {
                applyMode(manualMode);
            } else {
                KosMutexLock(&this->programMutex);
                applyMode(modeProgram[curProgramStep].modes);
                auto sleep = modeProgram[curProgramStep].workTimeSeconds;
                KosMutexUnlock(&this->programMutex);

                KosThreadSleep(sleep);
                curProgramStep = (curProgramStep + 1) % modeProgram.size();
            }
            KosThreadSleep(500);
        }
    }

    void
    onErrorReceived(nk_uint8_t id, const std::string &severity, const trafficlight_IDiagnostics_DirectionColor color) {
        failures[id].push({severity, {color.r, color.y, color.g}});
    }
};
