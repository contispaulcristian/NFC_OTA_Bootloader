# --- Microcontroller and Clock Speed ---
MCU = attiny1616
# Define clock speed 10MHz
F_CPU = 10000000UL

# --- Toolchain Definitions ---
CC = avr-gcc
OBJCOPY = avr-objcopy
# Variable for output directory
OUTPUT_DIR = prod

# --- Include Paths ---
INCLUDES = -I./include -I./src/hal -I./src/mcal_lite -I./src/secure

# --- Compiler Flags ---
CFLAGS = -Wall -Os -mmcu=$(MCU) -DF_CPU=$(F_CPU) -std=gnu11 $(INCLUDES)
CFLAGS += -ffunction-sections -fdata-sections -MMD -MP

# --- Linker Flags ---
LDFLAGS = -Wl,--gc-sections -Wl,--section-start=.text=0x0000

# --- Project Name and Source Files ---
TARGET = $(OUTPUT_DIR)/bootloader
SRCS = src/main.c \
       src/mcal_lite/clk_drv.c \
       src/mcal_lite/i2c_drv.c \
       src/mcal_lite/port_drv.c \
	   src/mcal_lite/nvm_drv.c \
	   #src/hal/is31fl3730_driver.c
       #src/hal/st25dv64k_driver.c \
	   src/secure/ota_updater.c
# WARNING! DO NOT COMPILE! src/fuses.c

# --- Build Rules ---
.PHONY: all clean flash directories size_report write_fuses

all: directories $(TARGET).hex
	@echo "-----------------------------------------------------"
	@echo "|   BUILD COMPLETE: $(TARGET).hex is ready.   |"
	@echo "-----------------------------------------------------"

# Create the prod directory if it doesn't exist
directories:
	@echo "Creating output directory: $(OUTPUT_DIR)..."
	@mkdir -p $(OUTPUT_DIR)

$(TARGET).elf: $(SRCS)
	@echo "Compiling C source files for $(MCU)..."
	@$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET).elf $(SRCS)

$(TARGET).hex: $(TARGET).elf
	@echo "Extracting executable hex from elf..."
	@$(OBJCOPY) -O ihex -R .eeprom -R .fuse -R .lock -R .signature $(TARGET).elf $(TARGET).hex


size_report: $(TARGET).elf
	@echo "-----------------------------------------------------------------------"
	@echo "  ADDRESS      SIZE (HEX)    SIZE (DEC)    TYPE    NAME"
	@echo "-----------------------------------------------------------------------"
	@avr-nm --print-size --size-sort --reverse-sort $(TARGET).elf | \
	grep -i ' [tTrRdD] ' | \
	awk '{ \
		addr = $$1; \
		size_hex = "0x"$$2; \
		size_dec = sprintf("%d", size_hex); \
		sum += size_dec; \
		printf "  %-12s %-12s %-12s %-6s %s\n", addr, size_hex, size_dec, $$3, $$4 \
	} \
	END { \
		printf "-----------------------------------------------------------------------\n"; \
		printf "  [1] SYMBOL LOGIC SIZE:    %d bytes\n", sum; \
	}'
	@# [2] Calculate the 'Write' size (Binary payload including Vector Table)
	@$(eval WRITE_SIZE=$(shell avr-size -A $(TARGET).elf | grep -E '\.text|\.data|\.rodata' | awk '{s+=$$2} END {print s}'))
	@echo "  [2] ESTIMATED WRITE SIZE: $(WRITE_SIZE) bytes (Payload + Vectors)"
	@# [3] Calculate the 'Verify' size (Aligned to 2-byte Word for ATtiny)
	@$(eval VERIFY_SIZE=$(shell echo $$(( ($(WRITE_SIZE) + 1) / 2 * 2 )) ))
	@echo "  [3] ESTIMATED VERIFY SIZE: $(VERIFY_SIZE) bytes (Word Aligned)"
	@echo "-----------------------------------------------------------------------"
	@echo "  FLASH USAGE: $(shell echo "scale=2; $(VERIFY_SIZE) / 163.84" | bc)% of 16KB"
	@echo "-----------------------------------------------------------------------"
	@echo "-----------------------------------------------------------------------"
	@echo "Section Summary (The Physical Reality):"
	@echo "-----------------------------------------------------------------------"
	@# Use printf and column to create perfect alignment
	@(printf "SECTION SIZE(DEC) SIZE(HEX) START_ADDR\n"; \
	  avr-size -A $(TARGET).elf | grep -E '(\.text|\.data|\.rodata|Total)' | \
	  awk '{ printf "%s %d 0x%x %s\n", $$1, $$2, $$2, $$3 }') | column -t
	@echo "-----------------------------------------------------------------------"
	@echo "-----------------------------------------------------------------------"


flash: $(TARGET).hex
	@echo "Connecting to Atmel-ICE and flashing $(MCU)..."
	@avrdude -c atmelice_updi -p $(MCU) -v -U flash:w:$(TARGET).hex:i && \
	(echo "---------------------------------------------------------"; \
	 echo "|   SUCCESS: Firmware flashed to $(MCU) successfully.   |"; \
	 echo "---------------------------------------------------------") || \
	(echo "-------------------------------------------------------"; \
	 echo "|   ERROR: Flashing failed. Check your UPDI wiring!   |"; \
	 echo "-------------------------------------------------------"; \
	 exit 1)

write_fuses:
	@echo "-----------------------------------------------------------"
	@echo "| WARNING: You are about to modify chip hardware fuses!   |"
	@echo "| Only proceed if you are sure you need to change memory  |"
	@echo "| partitioning. Are you sure? (Ctrl+C to abort)           |"
	@echo "-----------------------------------------------------------"
	@sleep 3
	@echo "Connecting to Atmel-ICE and writing fuses to $(MCU)..."
	@avrdude -c atmelice_updi -p $(MCU) -U fuses:w:$(TARGET).elf:e && \
	(echo "---------------------------------------------------------"; \
	 echo "|   SUCCESS: Fuses written to $(MCU) successfully.      |"; \
	 echo "---------------------------------------------------------") || \
	(echo "-------------------------------------------------------"; \
	 echo "|   ERROR: Fuse writing failed. Check UPDI wiring!    |"; \
	 echo "-------------------------------------------------------"; \
	 exit 1)

clean:
	@echo "Cleaning up build artifacts in $(OUTPUT_DIR)..."
	@rm -rf $(OUTPUT_DIR)
	@echo "-----------------------"
	@echo "|   Clean complete.   |"
	@echo "-----------------------"
