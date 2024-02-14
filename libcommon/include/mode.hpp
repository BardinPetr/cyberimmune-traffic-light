#ifndef TRAFFIC_LIGHT_MODE_HPP
#define TRAFFIC_LIGHT_MODE_HPP

#include <sstream>
#include "trafficlight/GPIO.edl.h"
#include "trafficlight/ILightMode.idl.h"

const std::pair<uint32_t, char> types[] = {
        {trafficlight_ILightMode_ModeRed,    'R'},
        {trafficlight_ILightMode_ModeYellow, 'Y'},
        {trafficlight_ILightMode_ModeGreen,  'G'},
        {trafficlight_ILightMode_ModeBlink,  'B'},
};

std::string mode_to_string(uint32_t mode) {
    std::stringstream ss;
    for (auto [mask, name]: types)
        ss << (mode & mask ? name : '-');
    ss << (mode & (trafficlight_ILightMode_ModeYellow | trafficlight_ILightMode_ModeGreen) ? "|^" : "|x");
    return ss.str();
}

#endif //TRAFFIC_LIGHT_MODE_HPP
