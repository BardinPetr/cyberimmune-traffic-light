project(control)

include(platform/nk)

set(CMAKE_CXX_STANDARD 20)

nk_build_edl_files(
        control_edl_files
        NK_MODULE "trafficlight"
        EDL "${CMAKE_SOURCE_DIR}/resources/edl/Control.edl"
)

add_executable(Control "src/main.cpp")
add_dependencies(Control control_edl_files trafficlight_idl)

target_link_libraries(Control PRIVATE common)
