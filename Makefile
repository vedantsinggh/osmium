KERNEL_DIR := kernel
BUILD_DIR := build
ROOTFS := rootfs

INIT_BIN := init/init
CELL_BIN := utils/cell/cell
ECHO_BIN := utils/echo/echo
INITRAMFS := $(BUILD_DIR)/initramfs.cpio.gz
KERNEL_IMAGE := $(KERNEL_DIR)/arch/x86/boot/bzImage

.PHONY: all init cell echo rootfs initramfs run clean

all: initramfs

init:
	$(MAKE) -C init

cell:
	$(MAKE) -C utils/cell

echo:
	$(MAKE) -C utils/echo

rootfs: init cell echo
	mkdir -p $(BUILD_DIR)/rootfs
	rm -rf $(BUILD_DIR)/rootfs/*
	cp -r $(ROOTFS)/* $(BUILD_DIR)/rootfs/ || true
	mv $(INIT_BIN) $(BUILD_DIR)/rootfs
	mv $(CELL_BIN) $(BUILD_DIR)/rootfs/bin
	mv $(ECHO_BIN) $(BUILD_DIR)/rootfs/bin

initramfs: rootfs
	cd $(BUILD_DIR)/rootfs && \
	find . | cpio -o -H newc | gzip > ../initramfs.cpio.gz

run: initramfs
	qemu-system-x86_64 \
	-kernel $(KERNEL_IMAGE) \
	-initrd $(INITRAMFS) \
	-nographic \
	-append "console=ttyS0 rdinit=/init"

clean:
	rm -rf $(BUILD_DIR)
	$(MAKE) -C init clean
	$(MAKE) -C cell clean
