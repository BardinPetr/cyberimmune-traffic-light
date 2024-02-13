project(gpio_mock_driver)

include(platform/nk)

set(CMAKE_CXX_STANDARD 20)

nk_build_edl_files(
        gpio_mock_edl
        CDL_TARGET gpio_mock_cdl
        NK_MODULE "gpio_mock_driver"
        EDL "resources/GPIO.edl"
)

file(GLOB SRC CONFIGURE_DEPENDS "**/*.cpp" "**/*.h")
include_directories("include")

add_executable(GPIOMockDriver ${SRC})
add_dependencies(GPIOMockDriver gpio_mock_idl gpio_mock_edl)

target_link_libraries(GPIOMockDriver PRIVATE common)