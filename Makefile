KERNEL_DIR := kernel
BUILD_DIR := build
ROOTFS := rootfs

INIT_BIN := init/init

UTIL_DIR := utils

UTIL_SUBDIRS := $(wildcard $(UTIL_DIR)/*/)
UTILS := $(notdir $(patsubst %/,%,$(UTIL_SUBDIRS)))

UTIL_BINS := $(foreach u,$(UTILS),$(UTIL_DIR)/$(u)/$(u))

INITRAMFS := $(BUILD_DIR)/initramfs.cpio.gz
KERNEL_IMAGE := $(KERNEL_DIR)/arch/x86/boot/bzImage

.PHONY: all init utils rootfs initramfs run clean debug

all: initramfs

init:
	$(MAKE) -C init

utils:
	@for u in $(UTILS); do \
		echo "[BUILD] $$u"; \
		$(MAKE) -C $(UTIL_DIR)/$$u || exit 1; \
	done

rootfs: init utils
	@echo "[ROOTFS] Preparing"
	mkdir -p $(BUILD_DIR)/rootfs/bin
	rm -rf $(BUILD_DIR)/rootfs/*
	cp -r $(ROOTFS)/* $(BUILD_DIR)/rootfs/ 2>/dev/null || true

	mv $(INIT_BIN) $(BUILD_DIR)/rootfs/init

	@for u in $(UTILS); do \
		mv $(UTIL_DIR)/$$u/$$u $(BUILD_DIR)/rootfs/bin/ || exit 1; \
	done

initramfs: rootfs
	@echo "[INITRAMFS] Packing"
	cd $(BUILD_DIR)/rootfs && \
	find . | cpio -o -H newc | gzip > ../initramfs.cpio.gz

run: initramfs
	qemu-system-x86_64 \
	-kernel $(KERNEL_IMAGE) \
	-initrd $(INITRAMFS) \
	-nographic \
	-append "console=ttyS0 rdinit=/init quiet loglevel=3"

clean:
	@echo "[CLEAN]"
	rm -rf $(BUILD_DIR)
	$(MAKE) -C init clean
	@for u in $(UTILS); do \
		$(MAKE) -C $(UTIL_DIR)/$$u clean || true; \
	done

debug:
	@echo "UTILS: $(UTILS)"
	@echo "UTIL_BINS: $(UTIL_BINS)"
