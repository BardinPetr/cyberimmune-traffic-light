#include "log.hpp"

void init_logging(const std::string &name) {
    L::set_default_logger(L::stderr_logger_mt(name));
    L::set_pattern("[%-5!n] [%l] %v");
    L::info("Started");
}
