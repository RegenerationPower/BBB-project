DESCRIPTION = "Custom Device Tree Overlay"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

inherit devicetree

SRC_URI = "file://button-overlay.dts"

COMPATIBLE_MACHINE = "beaglebone"

