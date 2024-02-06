project(gpio)

include(platform/nk)

set(CMAKE_CXX_STANDARD 20)

nk_build_edl_files(
        trafficlight_gpio_edl
        CDL_TARGET trafficlight_cdl
        NK_MODULE "trafficlight"
        EDL "${CMAKE_SOURCE_DIR}/resources/edl/GPIO.edl"
)

file(GLOB SRC CONFIGURE_DEPENDS "**/*.cpp" "**/*.h")
include_directories("include")

add_executable(GPIO ${SRC})
add_dependencies(GPIO trafficlight_gpio_edl trafficlight_idl)

target_link_libraries(GPIO PRIVATE common)