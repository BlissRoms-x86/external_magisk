

B_MAGISK := 1

ifeq ($(TARGET_ARCH),arm mips x86)
BB_INIT := 1
PRODUCT_PACKAGES += \
	magiskinit
else ifeq ($(TARGET_ARCH),arm64 x86_64)
B_INIT64 := 1
PRODUCT_PACKAGES += \
	magiskinit64
else
BB_INIT := 1
PRODUCT_PACKAGES += \
	magiskinit
endif

B_BOOT := 1
# B_BB := 1
B_TEST := 1


PRODUCT_PACKAGES += \
	magisk \
	magiskboot \
	busybox \
	libsystemproperties \
	libutils \
	resetprop \
	supolicy \
