CC              := cc
LD              := ld
RM              := rm -f

SRC_DIR         := src
BUILD_DIR       := build
OUT_DIR         := bin

ARCH            := x86_64

NAME            := uefijitfuck
SOURCES         := $(wildcard $(SRC_DIR)/**/*.c) $(wildcard $(SRC_DIR)/*.c)
OBJS            := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCES))
TARGET_LIB      := $(BUILD_DIR)/$(NAME).so
TARGET          := $(OUT_DIR)/$(NAME).efi

EFIINC          := /usr/include/efi
EFIINCS         := -I$(EFIINC) -I$(EFIINC)/$(ARCH) -I$(EFIINC)/protocol
LIB             := /usr/lib
EFILIB          := /usr/lib
EFI_CRT_OBJS    := $(EFILIB)/crt0-efi-$(ARCH).o
EFI_LDS         := $(EFILIB)/elf_$(ARCH)_efi.lds

INCLUDE         := -I$(SRC_DIR)
CFLAGS          := $(INCLUDE) $(EFIINCS) -fno-stack-protector -fpic \
        -fshort-wchar -mno-red-zone -Wall -DEFI_FUNCTION_WRAPPER

LDFLAGS         := -nostdlib -znocombreloc -T $(EFI_LDS) -shared \
        -Bsymbolic -L $(EFILIB) -L $(LIB) $(EFI_CRT_OBJS)


all: $(TARGET)

$(TARGET): $(TARGET_LIB)
	@mkdir -p "$(@D)"
	objcopy -j .text -j .sdata -j .data -j .dynamic \
		-j .dynsym  -j .rel -j .rela -j .reloc \
		--target=efi-app-$(ARCH) $^ $@

$(TARGET_LIB): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $@ -lefi -lgnuefi

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p "$(@D)"
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) -r $(BUILD_DIR)/ $(OUT_DIR)/


.PHONY: clean all
.SECONDARY: $(OBJS)
