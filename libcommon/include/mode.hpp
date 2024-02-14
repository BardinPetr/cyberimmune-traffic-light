#ifndef TRAFFIC_LIGHT_MODE_HPP
#define TRAFFIC_LIGHT_MODE_HPP

#include <sstream>
#include "trafficlight/GPIO.edl.h"
#include "trafficlight/ILightMode.idl.h"

namespace trafficlight {
    enum ModeType {
        Red = trafficlight_ILightMode_ModeRed,
        Yellow = trafficlight_ILightMode_ModeYellow,
        Green = trafficlight_ILightMode_ModeGreen,
        Blink = trafficlight_ILightMode_ModeBlink,
    };

    const std::pair<uint32_t, char> types[] = {
            {Red,    'R'},
            {Yellow, 'Y'},
            {Green,  'G'},
            {Blink,  'B'},
    };

    std::string mode_to_string(uint32_t mode) {
        std::stringstream ss;
        for (auto [mask, name]: types)
            ss << (mode & mask ? name : '-');
        ss << (mode & (Yellow | Green) ? "|^" : "|x");
        return ss.str();
    }
}


#endif //TRAFFIC_LIGHT_MODE_HPP
