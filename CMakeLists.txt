cmake_minimum_required(VERSION 3.12)
project(traffic-light)

include(platform)
initialize_platform(FORCE_STATIC)

add_subdirectory(resources)
add_subdirectory(tl_link)
add_subdirectory(tl_control)
add_subdirectory(tl_diagnostics)
add_subdirectory(tl_gpio)
add_subdirectory(einit)
