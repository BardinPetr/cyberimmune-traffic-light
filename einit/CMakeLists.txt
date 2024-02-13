project(einit)

include(platform/image)

set(CMAKE_CXX_STANDARD 11)

set(ENTITIES Link Control Diagnostics GPIO)
set(SECURITY_PSL_FILE "src/security.psl.in")

build_kos_hw_image(kos-image
        EINIT_ENTITY EinitHw
        CONNECTIONS_CFG "src/init.yaml.in"
        SECURITY_PSL ${SECURITY_PSL_FILE}
        ${ENTITIES}
)

build_kos_qemu_image(kos-qemu-image
        EINIT_ENTITY EinitQemu
        CONNECTIONS_CFG "src/init.yaml.in"
        SECURITY_PSL ${SECURITY_PSL_FILE}
        ${ENTITIES}
)
