project(diagnostics)

include(platform/nk)

nk_build_edl_files(
        diagnostics_edl_files
        NK_MODULE "trafficlight"
        CDL_TARGET trafficlight_cdl
        EDL "${CMAKE_SOURCE_DIR}/resources/edl/Diagnostics.edl"
)

add_executable(Diagnostics "src/main.cpp"
        src/DiagnosticsController.hpp)
add_dependencies(Diagnostics diagnostics_edl_files trafficlight_idl)

target_link_libraries(Diagnostics PRIVATE libcommon)
