project(libcommon)

include(platform/nk)

include_directories("../include" "./include")

add_library(libcommon
        src/log.cpp
        src/ILightMode.idl.cpp
        src/ITargetState.idl.cpp
        src/IDiagnostics.idl.cpp
        src/IExternalControl.idl.cpp
        include/ILightMode.idl.hpp
        include/ITargetState.idl.hpp
        include/IDiagnostics.idl.hpp
        include/IExternalControl.idl.hpp
        include/mode.hpp
)

add_dependencies(libcommon trafficlight_gpio_edl diagnostics_edl_files control_edl_files trafficlight_idl)

target_link_libraries(libcommon PUBLIC spdlog::spdlog libxnk)
