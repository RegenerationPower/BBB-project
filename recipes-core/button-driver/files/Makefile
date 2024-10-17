obj-m := button_driver.o

ARCH ?= arm
CROSS_COMPILE ?= $(CROSS_COMPILE)
# Hardcoded shit
#KERN_DIR ?= ~/Embedded_Linux_BBB/build_bbb_custom/tmp/work/beaglebone-poky-linux-gnueabi/linux-bb.org/6.1.80+git/#build
HOST_KERN_DIR = /lib/modules/$(shell uname -r)/build/

all:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(HOST_KERN_DIR) M=$(shell pwd) modules
clean:
	#make -C $(HOST_KERN_DIR) M=$(shell pwd) clean
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(HOST_KERN_DIR) M=$(shell pwd) clean
help:
	@echo "Targets:"
	@echo "  all      - Build the module"
	@echo "  clean    - Clean the module"

