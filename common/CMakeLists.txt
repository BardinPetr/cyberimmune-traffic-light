project(common)

set(CMAKE_CXX_STANDARD 11)

file(GLOB SRC CONFIGURE_DEPENDS "**/*.cpp" "../include/*.hpp")
include_directories("../include")

add_library(common ${SRC})

target_link_libraries(common PUBLIC spdlog::spdlog)
