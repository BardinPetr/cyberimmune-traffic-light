project(diagnostics)

include(platform/nk)

set(CMAKE_CXX_STANDARD 11)

nk_build_edl_files(
        diagnostics_edl_files
        NK_MODULE "trafficlight"
        EDL "${CMAKE_SOURCE_DIR}/resources/edl/Diagnostics.edl"
)

add_executable(Diagnostics "src/main.cpp")
add_dependencies(Diagnostics diagnostics_edl_files)
