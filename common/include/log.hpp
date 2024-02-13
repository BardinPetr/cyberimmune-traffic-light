#ifndef TRAFFIC_LIGHT_LOG_H
#define TRAFFIC_LIGHT_LOG_H

#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/spdlog.h>
#include <string>

namespace L = spdlog;

void init_logging(const std::string &name);

#endif //TRAFFIC_LIGHT_LOG_H
