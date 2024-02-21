#ifndef TRAFFICLIGHT_LOGGER_H
#define TRAFFICLIGHT_LOGGER_H

#include <iostream>
#include <fstream>
#include <coresrv/time/time_api.h>

class Logger {

public:
    Logger() = default;

    void append(const nk_uint8_t id, const nk_uint32_t mode) {
        std::string mode_str = mode_to_string(mode);

        RtlTimeSpec time;
        KnGetSystemTime(&time);

        std::ofstream logFile("/log", std::ios_base::app);
        logFile << "[" << time.sec << "] ID: " << (int) id << "  MODE: " << mode_str << "\n";
        logFile.close();
    }
};

#endif //TRAFFICLIGHT_LOGGER_H
