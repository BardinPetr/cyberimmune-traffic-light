#ifndef TRAFFIC_LIGHT_XNKARENA_HPP
#define TRAFFIC_LIGHT_XNKARENA_HPP

#include <coresrv/sl/sl_api.h>
#include <coresrv/nk/transport-kos.h>

template<int size>
class XNkArena : public nk_arena {
private:
    nk_uint8_t buffer[size]{};
public:
    explicit XNkArena() : nk_arena() {
        start = &buffer;
        current = &buffer;
        end = &buffer + size;
    }

    void reset() {
        current = start;
    }
};

#endif //TRAFFIC_LIGHT_XNKARENA_HPP
