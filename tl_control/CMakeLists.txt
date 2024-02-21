project(control)

include(platform/nk)

nk_build_edl_files(
        control_edl_files
        NK_MODULE "trafficlight"
        CDL_TARGET trafficlight_cdl
        EDL "${CMAKE_SOURCE_DIR}/resources/edl/Control.edl"
)

add_executable(Control
        src/main.cpp
        src/StateController.hpp
        src/DiagnosticsReceiver.hpp
        src/Logger.hpp
)
add_dependencies(Control control_edl_files trafficlight_idl)

target_link_libraries(Control PRIVATE libcommon ${vfs_CLIENT_LIB})
