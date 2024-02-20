cmake_minimum_required(VERSION 3.12)
project(trafficlight)

set(CMAKE_CXX_STANDARD 20)

include(platform)
initialize_platform(FORCE_STATIC)

include(platform/install)
include(platform/options)
include(platform/nk)

find_package(spdlog REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)

find_package(vfs REQUIRED)
include_directories(${vfs_INCLUDE})
find_package(rump REQUIRED COMPONENTS DHCPCD_ENTITY)
include_directories(${rump_INCLUDE})
find_package(precompiled_vfs REQUIRED)
include_directories(${precompiled_vfs_INCLUDE})

option(DHCPCD_FALLBACK "Configuration by /etc/dhcpcd.conf" OFF)
option(DHCPCD_DYNAMIC "Configuration by /etc/dhcpcd_dynamic.config" OFF)
option(DHCPCD_STATIC "Configuration by /etc/dhcpcd_static.config" ON)


include_directories(${CMAKE_CURRENT_BINARY_DIR} libcommon/include libxnk/include)
set(RESOURCES ${CMAKE_SOURCE_DIR}/resources)

add_subdirectory(resources)
add_subdirectory(libxnk)
add_subdirectory(libcommon)
add_subdirectory(tl_link)
add_subdirectory(tl_control)
add_subdirectory(tl_diagnostics)
add_subdirectory(tl_gpio)
add_subdirectory(einit)
