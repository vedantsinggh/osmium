KERNEL_DIR := kernel
BUILD_DIR := build
ROOTFS := rootfs

INIT_BIN := init/init
INITRAMFS := $(BUILD_DIR)/initramfs.cpio.gz
KERNEL_IMAGE := $(KERNEL_DIR)/arch/x86/boot/bzImage

.PHONY: all init rootfs initramfs run clean

all: initramfs

init:
	$(MAKE) -C init

rootfs: init
	mkdir -p $(BUILD_DIR)/rootfs
	rm -rf $(BUILD_DIR)/rootfs/*
	cp -r $(ROOTFS)/* $(BUILD_DIR)/rootfs/ || true
	cp $(INIT_BIN) $(BUILD_DIR)/rootfs/init

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
