DESCRIPTION = "Button driver for Beagle Bone Black"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://button_driver.c \
            file://Makefile"
            
S = "${WORKDIR}"

inherit module

do_compile() {
    oe_runmake -C ${STAGING_KERNEL_DIR} M=${S} modules
}

do_install() {
    install -d ${D}/lib/modules/${KERNEL_VERSION}/extra/
    install -m 644 ${S}/button_driver.ko ${D}/lib/modules/${KERNEL_VERSION}/extra/
}

#FILES:${PN} = "/lib/modules/${KERNEL_VERSION}/extra/button_driver.ko"

