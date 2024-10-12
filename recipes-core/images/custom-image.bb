require recipes-core/images/core-image-minimal.bb

DESCRIPTION = "Custom Image for practice purposes"

IMAGE_INSTALL += " \
    custom-overlay \
    "

IMAGE_BOOT_FILES += "devicetree/button-overlay.dtbo;button-overlay.dtbo"

