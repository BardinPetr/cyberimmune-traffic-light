project(gpio)

include(platform/nk)

set(CMAKE_CXX_STANDARD 11)

nk_build_edl_files(
        trafficlight_gpio_edl
        CDL_TARGET trafficlight_cdl
        NK_MODULE "trafficlight"
        EDL "${CMAKE_SOURCE_DIR}/resources/edl/GPIO.edl"
)

add_executable(GPIO "src/main.cpp")
add_dependencies(GPIO trafficlight_gpio_edl)

target_link_libraries(GPIO PRIVATE common)