project(einit)

include(platform/image)

set(ENTITIES
        Link
        Control
        Diagnostics
        GPIO
)

set(SECURITY_PSL_FILE "src/security.psl")

if (ROOTFS_SDCARD)
    set(ROOTFS_ENTITY "kl.VfsSdCardFs")
    set(ROOTFS_DEVICE "sdcard")
    set(FS_BACKEND ${precompiled_vfsVfsSdCardFs})
    set(FS_IMAGE sdcard0.img)
    set(FS_IMG_FORMAT fat32)
    set(FS_IMG_SIZE 64)
    set(QEMU_FLAGS "-drive file=${FS_IMAGE},if=sd,format=raw")
else ()
    set(ROOTFS_ENTITY "kl.VfsRamFs")
    set(ROOTFS_DEVICE "ramfs")
    set(FS_BACKEND ${precompiled_vfsVfsRamFs})
    set(FS_IMAGE ramdisk0.img)
    set(FS_IMG_FORMAT ext4)
    set(FS_IMG_SIZE 2)
    set(FILES ${CMAKE_BINARY_DIR}/einit/${FS_IMAGE})
endif ()

list(APPEND ENTITIES
        rump::dhcpcd_entity
        ${precompiled_vfsVfsNet}
        ${precompiled_vfsVfsRamFs}
        ${FS_BACKEND}
)

set_target_properties(${FS_BACKEND} PROPERTIES
        EXTRA_ARGS "\
    - -l
    - devfs /dev devfs 0")

if (DHCPCD_FALLBACK)
    set(DHCPCD_CONFIG_FILE "/etc/dhcpcd.conf")
    message("Used dynamic network configuration with fallback to static configuration by ${ROOTFS_DEVICE}: ${DHCPCD_CONFIG_FILE}")
elseif (DHCPCD_DYNAMIC)
    set(DHCPCD_CONFIG_FILE "/etc/dhcpcd_dynamic.conf")
    message("Used dynamic network configuration by ${ROOTFS_DEVICE}: ${DHCPCD_CONFIG_FILE}")
elseif (DHCPCD_STATIC)
    set(DHCPCD_CONFIG_FILE "/etc/dhcpcd_static.conf")
    message("Used static network configuration by ${ROOTFS_DEVICE}: ${DHCPCD_CONFIG_FILE}")
else ()
    set(DHCPCD_CONFIG_FILE "/etc/dhcpcd.conf")
    message("Used default dynamic network configuration with fallback to static configuration by ${ROOTFS_DEVICE}: ${DHCPCD_CONFIG_FILE}")
endif ()

# Redefine DHCPCD entity connections
set_target_properties(${rump_DHCPCD_ENTITY} PROPERTIES
        DEPENDS_ON_ENTITY ""
        EXTRA_CONNECTIONS "\
  - target: kl.VfsNet
    id: kl.VfsNet
  - target: ${ROOTFS_ENTITY}
    id: ${ROOTFS_ENTITY}"
        EXTRA_ENV "\
    VFS_FILESYSTEM_BACKEND: client{fs->net}:${ROOTFS_ENTITY}
    VFS_NETWORK_BACKEND: client:kl.VfsNet"
        EXTRA_ARGS "\
    - '-4'
    - -f
    - ${DHCPCD_CONFIG_FILE}")

# Add a target to create disk image.
add_custom_command(
        OUTPUT ${FS_IMAGE}
        COMMAND ${KL_SDK_ROOT_PATH}/common/prepare_hdd_img.sh
        -d ${CMAKE_SOURCE_DIR}/resources/hdd
        -s ${FS_IMG_SIZE}
        -f ${FS_IMG_FORMAT}
        -img ${FS_IMAGE}
        DEPENDS ${CMAKE_SOURCE_DIR}/resources/hdd
)

add_custom_target(${FS_IMAGE} ALL
        DEPENDS ${FS_IMAGE}
)

# KasperskyOS image for the target hardware platform.
build_kos_hw_image(kos-image
        EINIT_ENTITY EinitHw
        CONNECTIONS_CFG "src/init.yaml.in"
        SECURITY_PSL ${SECURITY_PSL_FILE}
        IMAGE_FILES ${ENTITIES} ${FILES})

if (NOT ROOTFS_SDCARD)
    add_dependencies(kos-image ${FS_IMAGE})
endif ()

# KasperskyOS image for QEMU with simulation targets (sim, gdbsim, gdb).
build_kos_qemu_image(kos-qemu-image
        EINIT_ENTITY EinitQemu
        CONNECTIONS_CFG "src/init.yaml.in"
        SECURITY_PSL ${SECURITY_PSL_FILE}
        QEMU_FLAGS "${QEMU_FLAGS}"
        QEMU_DEPENDENCIES "${QEMU_DEPENDENCIES}"
        IMAGE_FILES ${ENTITIES} ${FILES})

add_dependencies(kos-qemu-image ${FS_IMAGE})
