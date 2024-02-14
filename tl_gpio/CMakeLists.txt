project(gpio)

include(platform/nk)

nk_build_edl_files(
        trafficlight_gpio_edl
        CDL_TARGET trafficlight_cdl
        NK_MODULE "trafficlight"
        EDL "${CMAKE_SOURCE_DIR}/resources/edl/GPIO.edl"
        NK_FLAGS ""
)

file(GLOB SRC CONFIGURE_DEPENDS "**/*.cpp" "**/*.h")
include_directories("include")

add_executable(GPIO ${SRC}
        src/LightModeController.hpp)
add_dependencies(GPIO trafficlight_gpio_edl trafficlight_idl)

target_link_libraries(GPIO PRIVATE libcommon)