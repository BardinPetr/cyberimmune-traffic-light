project(resources)

include(platform/nk)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(NK_MOD "trafficlight")

nk_build_idl_files(
        trafficlight_idl
        NK_MODULE ${NK_MOD}
        IDL "idl/ILightMode.idl"
)

nk_build_cdl_files(
        trafficlight_cdl
        IDL_TARGET trafficlight_idl
        NK_MODULE ${NK_MOD}
        CDL "cdl/CLightMode.cdl"
)
