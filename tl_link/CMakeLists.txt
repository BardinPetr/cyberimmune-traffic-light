project(Link)

include(platform/nk)

set(CMAKE_CXX_STANDARD 11)

nk_build_edl_files(
        link_edl_files
        NK_MODULE "trafficlight"
        EDL "${CMAKE_SOURCE_DIR}/resources/edl/Link.edl"
)

add_executable(Link "src/main.cpp")
add_dependencies(Link link_edl_files)
