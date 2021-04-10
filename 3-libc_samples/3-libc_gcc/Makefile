
CC      = arm-none-eabi-gcc
LD      = arm-none-eabi-ld
AR      = arm-none-eabi-ar
OBJCOPY = arm-none-eabi-objcopy
BJDUMP = arm-none-eabi-objdump


INCLUDE:=include

OBJS:=h3_startup.o \
			uart.o\
			delay.o\
			led.o\
			syscall.o\
			main.o
CFLAGS:=  -mcpu=cortex-a7  -mfloat-abi=softfp -mfpu=vfpv4-d16  -O0 -g -Wall   -std=c11 
# 
#LKFLAGS:= -o  -nostartfiles  -mcpu=cortex-a7 
LIBS:=
#-L/usr/lib/arm-none-eabi/lib/armv7-ar/thumb   #-lc -lm -lg  -lgcc_pic -lgcc_eh -lgcov


INC_TOP=$(shell pwd)

INCLUDEDIR 	:= -Iinclude


CPPFLAGS   	:=   $(INCLUDEDIR) 

export SDKROOT	CC AR LD OBJCOPY OBJDUMP INCLUDEDIR CFLAGS CPPFLAGS 

TARGET=boot.bin
LDSCRIPT:=h3_dram.ld
#LDSCRIPT:=boot.ld 
#

.PHONY :  all 

all:$(TARGET)
	echo $(INC_TOP)
$(TARGET): $(OBJS) 
	@echo $(OBJS)
	${CC} -o boot.elf $^ $(LIBS) -g -T $(LDSCRIPT)   -nostartfiles -Wl,--gc-sections  -specs=rdimon.specs
	${OBJCOPY} -O binary -S boot.elf $@
#	${OBJDUMP} -D boot.elf > boot.dis
#	./tool/mksunxiboot $(TARGET) $(TARGET)



%.o:%.c
	${CC} $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

%.o:%.S
	${CC} $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

lib/libc.a:
	make -C libxx

clean:
	rm -f boot.bin *.elf *.dis *.o *~
	make -C lib clean



