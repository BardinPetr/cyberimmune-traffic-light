project(common)

include(platform/nk)

include_directories("../include" "./include")

add_library(common ${SRC}
        src/log.cpp
        include/NkArena.hpp
        src/trafficlight/ILightMode.idl.cpp
        include/ILightMode.idl.hpp)

add_dependencies(common trafficlight_gpio_edl trafficlight_idl)

target_link_libraries(common PUBLIC spdlog::spdlog)
