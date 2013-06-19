#
#             LUFA Library
#     Copyright (C) Dean Camera, 2012.
#
#  dean [at] fourwalledcubicle [dot] com
#           www.lufa-lib.org
#
# --------------------------------------
#         LUFA Project Makefile.
# --------------------------------------

MCU          = at90usb1287
ARCH         = AVR8
BOARD        = NONE
F_CPU        = 16000000
F_USB        = $(F_CPU)
OPTIMIZATION = s
TARGET       = BootloaderCDC
SRC          = $(TARGET).c Descriptors.c BootloaderAPI.c BootloaderAPITable.S $(LUFA_SRC_USB)
LUFA_PATH    = ../../../cores/AT90USB/LUFA
LUFA_BUILD   = ../..
CC_FLAGS     = -DUSE_LUFA_CONFIG_HEADER -IConfig/ -DBOOT_START_ADDR=$(BOOT_START_OFFSET)
LD_FLAGS     = -Wl,--section-start=.text=$(BOOT_START_OFFSET) $(BOOT_API_LD_FLAGS)

# Flash size and bootloader section sizes of the target, in KB. These must
# match the target's total FLASH size and the bootloader size set in the
# device's fuses.
FLASH_SIZE_KB         = 128
BOOT_SECTION_SIZE_KB  = 4

# Bootloader address calculation formulas (requires the "bc" unix utility)
# Do not modify these macros, but rather modify the depedant values above.
BOOT_START_OFFSET     = 0x$(shell echo "obase=16; ($(FLASH_SIZE_KB) - $(BOOT_SECTION_SIZE_KB)) * 1024" | bc)
BOOT_SEC_OFFSET       = 0x$(shell echo "obase=16; (($(FLASH_SIZE_KB) * 1024) - $(strip $(1)))" | bc)

# Bootloader linker section flags for relocating the API table sections to
# known FLASH addresses - these should not normally be user-edited.
BOOT_SECTION_LD_FLAG  = -Wl,--section-start=.apitable_$(strip $(1))=$(call BOOT_SEC_OFFSET, $(3)) -Wl,--undefined=BootloaderAPI_$(strip $(2))
BOOT_API_LD_FLAGS     = $(call BOOT_SECTION_LD_FLAG, trampolines, Trampolines, 96)
BOOT_API_LD_FLAGS    += $(call BOOT_SECTION_LD_FLAG, jumptable,   JumpTable,   32)
BOOT_API_LD_FLAGS    += $(call BOOT_SECTION_LD_FLAG, signatures,  Signatures,  8)

# Default target
all: $(PROGRAM)AT90usb128X.hex

#MX3 target
MX3: CC_FLAGS += -DBOARD_MX3
MX3: $(PROGRAM)AT90usb1287_MX3.hex

#MoCo Dev Board target
MOCO_DEV: CC_FLAGS += -DBOARD_MOCO_DEV
MOCO_DEV: $(PROGRAM)AT90usb1287_MoCo_Dev.hex 

#USBKey target
USBKEY: CC_FLAGS += -DBUSBKEY
USBKEY: $(PROGRAM)AT90usb1287_USBKey.hex 

# Include LUFA build script makefiles
include $(LUFA_BUILD)/Build/lufa_core.mk
include $(LUFA_BUILD)/Build/lufa_sources.mk
include $(LUFA_BUILD)/Build/lufa_build.mk
include $(LUFA_BUILD)/Build/lufa_cppcheck.mk
include $(LUFA_BUILD)/Build/lufa_doxygen.mk
include $(LUFA_BUILD)/Build/lufa_avrdude.mk
include $(LUFA_BUILD)/Build/lufa_atprogram.mk