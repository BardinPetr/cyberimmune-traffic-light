cmake_minimum_required(VERSION 3.12)
project(traffic-light)

set(CMAKE_CXX_STANDARD 20)

include(platform)
initialize_platform(FORCE_STATIC)

find_package(spdlog REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/common/include)

#add_subdirectory(gpio_mock_driver)
add_subdirectory(resources)
add_subdirectory(common)
add_subdirectory(tl_link)
add_subdirectory(tl_control)
add_subdirectory(tl_diagnostics)
add_subdirectory(tl_gpio)
add_subdirectory(einit)
