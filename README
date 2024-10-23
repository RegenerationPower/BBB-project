# Custom Yocto Image for BeagleBone Black

## Overview

This custom Yocto image is designed for the BeagleBone Black and is based on `core-image-minimal`. The image includes a GPIO-based button driver and a custom device tree overlay that configures a button on the BeagleBone Black. This setup provides a foundation for future projects requiring button input handling and custom driver development.

## Key Features

- **Button Driver (`button-driver`)**: A custom kernel module that handles GPIO button presses, mapped to the "Enter" key (`KEY_ENTER`). The driver uses the `gpio-keys` subsystem and handles button press events using GPIO interrupts.
- **Device Tree Overlay (`custom-overlay`)**: Configures the GPIO pin (P9_15, GPIO1_16) for the button input, along with a debounce interval.
- **Custom Image (`custom-image`)**: Integrates the button driver and device tree overlay, built on top of the core-image-minimal for the BeagleBone Black.

## Layers Included

The following layers are required to build the custom image:

- **Poky**: Core Yocto layer.
- **Meta-yocto-bsp**: Board support for Yocto.
- **Meta-arm and Meta-arm-toolchain**: Support for ARM platforms.
- **Meta-ti-bsp**: Texas Instruments BSP for BeagleBone Black.
- **Custom Layer (`custom_layer`)**: Contains the button driver, device tree overlay, and custom image recipes.

## Requirements

- **Target System**: BeagleBone Black
- **Yocto Version**: 5.0.4 (Scarthgap)
- **Kernel Version**: 6.1.80-ti  
- **Toolchain**: ARMv7 (hard-float)

## Build Instructions

1. **Clone the necessary repositories** and set up your Yocto environment.

2. **Add the custom layer**:
   ```bash
   bitbake-layers add-layer /path/to/custom_layer
   ```
   
3. **Configure the build by adding the following lines to your local.conf file:**:
   ```bash
    MACHINE ??= "beaglebone"
    PACKAGE_CLASSES ?= "package_deb"
   ```

4. **Build the custom image**:
   ```bash
   bitbake custom-image
   ```

## Flashing the Image

1. Once the build completes, locate the `custom-image-beaglebone.rootfs.wic` file in the `deploy-ti/images/beaglebone/` directory.
2. Use the following `dd` command to flash the image to a microSD card:
   ```bash
   sudo dd if=custom-image-beaglebone.rootfs.wic of=/dev/sdX status=progress bs=4096 && sync
   ```

## Post-Flashing Configuration

To apply the custom device tree overlay:

1. After flashing the image, mount the boot partition of the microSD card.
2. Open the `extlinux.conf` file located in the `/boot/extlinux/` directory.
3. Add the following line to configure the device tree overlay:
   ```bash
   FDTOVERLAYS ../button-overlay.dtbo
   ```
4. Save and close the file.
5. Insert the microSD card into the BeagleBone Black and boot the device.

## Button Driver Details

- **Driver Name**: `button_driver`
- **GPIO Pin**: P9_15 (GPIO1_16)
- **Button Action**: Reports button press events as `KEY_ENTER`.
- **Debounce**: Configured in the device tree overlay with a 50 ms debounce interval.

### Device Tree Overlay (`button-overlay.dts`)

This overlay configures the GPIO pin for the button:
```dts
fragment@1 {
    target = <&ocp>;
    __overlay__ {
        button {
		 	compatible = "gpio-keys";
            pinctrl-names = "default";
            pinctrl-0 = <&button_pins>;
			label = "Button 1";
			gpios = <&gpio1 16 GPIO_ACTIVE_LOW>;
			linux,code = <KEY_ENTER>;
			debounce-interval = <50>;
        };
    };
};
```

## Future Enhancements

This layer is a foundation for future projects, custom driver improvements are planned.
In the future, the process will be automated to avoid manual edits to the `extlinux.conf` file.
