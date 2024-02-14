project(libxnk CXX)
set(CMAKE_CXX_STANDARD 20)

include(platform/nk)

include_directories("./include")

add_library(libxnk
        include/XNkArena.hpp
        include/XNkKosTransport.hpp
        include/XNkKosTransportServer.hpp
)
set_target_properties(libxnk PROPERTIES LINKER_LANGUAGE CXX)
