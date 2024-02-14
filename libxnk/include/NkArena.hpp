#ifndef TRAFFIC_LIGHT_NKARENA_HPP
#define TRAFFIC_LIGHT_NKARENA_HPP

#include <coresrv/sl/sl_api.h>
#include <coresrv/nk/transport-kos.h>

template<int size>
class NkArena : public nk_arena {
private:
    nk_uint8_t buffer[size]{};
public:
    explicit NkArena() : nk_arena() {
        start = &buffer;
        current = &buffer;
        end = &buffer + size;
    }

    void reset() {
        current = start;
    }
};

#endif //TRAFFIC_LIGHT_NKARENA_HPP
