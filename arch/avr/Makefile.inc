ECHO = echo
RM = rm

export CC  = avr-gcc
export CXX = avr-g++
export AR  = avr-ar
MFLAGS =
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump

FORMAT = ihex
MSG_EEPROM = Creating load file for EEPROM:
MSG_CLEAN = Cleaning up in .:
MSG_ELF = Creating ELF file:

ISPTOOL    = avrisp
ISPPORT    = $(uartDev)
ISPSPEED   = -b 38400

export AVR_FREQ   = 20000000L

export MCU_TARGET = atmega328p
ISP_TARGET = m328
# LDSECTION  = --section-start=.text=0x3800

ISPFLASH    = avrdude -c $(ISPTOOL) -p $(ISP_TARGET) -P $(ISPPORT) $(ISPSPEED) -U flash:w:server.hex -U eeprom:w:server.eep
#ISPFLASH    = avrdude -c $(ISPTOOL) -p $(ISP_TARGET) -P $(ISPPORT) $(ISPSPEED) -U flash:w:$(PROGRAM)_$(TARGET).hex
# -U lock:w:0x0f:m

OBJ   = $(PROGRAM).o

DEFS       = $(DEBUG)
LIBS       =

OBJS += $(OBJ)

export OPTIMIZE += -Os
export CXXFLAGS += -g -Wall -Wundef -Werror -std=c++0x $(OPTIMIZE) -fno-implicit-templates -ffunction-sections -funsigned-char -funsigned-bitfields -fshort-enums -fdata-sections -ffreestanding -mcall-prologues -fno-tree-scev-cprop -fsplit-wide-types --param inline-call-cost=2
# -fno-inline-small-functions
# -frepo

CXXFLAGS += -mmcu=$(MCU_TARGET) -DF_CPU=$(AVR_FREQ) $(DEFS) $(IPATH)

export LDFLAGS  = -Wl,--relax,--gc-sections
# -Wl,--as-needed
# -Wl ,$(LDSECTION)

clean:
	@$(ECHO) $(MSG_CLEAN)
	 -$(RM) -f *.o *.elf *.lst *.map *.sym *.lss *.eep *.srec *.bin *.hex core $(OBJS)
#       -for d in $(DIRS); do (cd $$d; $(MAKE) clean ); done

fresh: clean all

program: force
	$(ISPFLASH)

%.lst: %.elf
	$(OBJDUMP) -h -S $< > $@

%.hex: %.elf
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

%.srec: %.elf
	$(OBJCOPY) -j .text -j .data -O srec $< $@

%.bin: %.elf
	$(OBJCOPY) -j .text -j .data -O binary $< $@

%.eep: %.elf
	@$(ECHO)
	@$(ECHO) $(MSG_EEPROM) $@
	-$(OBJCOPY) -j .eeprom --set-section-flags=.eeprom="alloc,load" \
	--change-section-lma .eeprom=0 -O $(FORMAT) $< $@
	avr-size $@

#atmega328: TARGET = atmega328
#atmega328: $(PROGRAM)_atmega328.hex

%.elf: $(OBJS)
	@$(ECHO)
	@$(ECHO) $(MSG_ELF) $@
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $(OBJS) $(LIBS) -Wl,-Map,$*.map
	avr-size $@

$(PROGRAM) : $(SRC) $(OBJS)
	@$(ECHO) "*** Building $@ ..."
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) -Wl,-Map,server.map --cref

.PHONY: all clean fresh force program

