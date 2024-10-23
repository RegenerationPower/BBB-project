obj-m := button_driver.o

ARCH ?= arm
CROSS_COMPILE ?= $(CROSS_COMPILE)
KERN_DIR = /lib/modules/$(shell uname -r)/build/

all:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERN_DIR) M=$(shell pwd) modules
clean:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERN_DIR) M=$(shell pwd) clean
help:
	@echo "Targets:"
	@echo "  all      - Build the module"
	@echo "  clean    - Clean the module"

