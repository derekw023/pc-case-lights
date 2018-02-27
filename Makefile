OBJECTS=pc_lights.o

GCC_DIR =  /opt/ti/mspgcc/bin
SUPPORT_FILE_DIRECTORY = /opt/ti/mspgcc/msp430-elf/include

DEVICE  = msp430g2553
CC      = $(GCC_DIR)/msp430-elf-gcc
GDB     = $(GCC_DIR)/msp430-elf-gdb

CFLAGS = -I $(SUPPORT_FILE_DIRECTORY) -T $(DEVICE).ld -mmcu=$(DEVICE) -mhwmult=none -O0
LFLAGS = -L $(SUPPORT_FILE_DIRECTORY)

all: ${OBJECTS}
	$(CC) $(CFLAGS) $(LFLAGS) $? -o $(DEVICE).elf

debug: all
	$(GDB) $(DEVICE).elf

clean: 
	rm ${OBJECTS} $(DEVICE).elf
