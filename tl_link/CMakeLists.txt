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
)
add_dependencies(Link link_edl_files trafficlight_idl)

target_link_libraries(Link
        libcommon
        rump::dhcpcd_client
        ${vfs_CLIENT_LIB}
        nlohmann_json::nlohmann_json
        ${Boost_LIBRARIES}
        Boost::json
)
set_target_properties(Link PROPERTIES ${vfs_ENTITY}_REPLACEMENT "")
