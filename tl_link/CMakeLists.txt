project(link)

include(platform/nk)

nk_build_edl_files(
        link_edl_files
        NK_MODULE "trafficlight"
        EDL "${CMAKE_SOURCE_DIR}/resources/edl/Link.edl"
)

add_executable(
        Link
        src/jsonbody.hpp
        src/httpclient.hpp
        src/client.hpp
        src/main.cpp
        src/sender.hpp
)
add_dependencies(Link link_edl_files trafficlight_idl)

target_link_libraries(Link
        libcommon
        ${vfs_CLIENT_LIB}
        ${Boost_LIBRARIES}
        Boost::json
)
set_target_properties(Link PROPERTIES ${vfs_ENTITY}_REPLACEMENT "")
