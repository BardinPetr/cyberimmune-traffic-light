project(resources)

include(platform/nk)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(NK_MOD "trafficlight")

file(GLOB IDL_FILES CONFIGURE_DEPENDS "idl/*.idl")
nk_build_idl_files(
        trafficlight_idl
        NK_MODULE ${NK_MOD}
        IDL ${IDL_FILES}
)

file(GLOB CDL_FILES CONFIGURE_DEPENDS "cdl/*.cdl")
nk_build_cdl_files(
        trafficlight_cdl
        IDL_TARGET trafficlight_idl
        NK_MODULE ${NK_MOD}
        CDL ${CDL_FILES}
)
