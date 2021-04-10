
CC      = arm-none-eabi-gcc
LD      = arm-none-eabi-ld
AR      = arm-none-eabi-ar
OBJCOPY = arm-none-eabi-objcopy
BJDUMP = arm-none-eabi-objdump

INCLUDE:=include

OBJS:= 		start.o \
			uart.o\
			delay.o\
			led.o\
			main.o\
			intc.o\
			lib/libc.a\
			cpu/libint.a
#CFLAGS:= -c  -marm -mcpu=cortex-a7  -  -O0 -g -Wall -nostdlib -marm 
# 
#LKFLAGS:= -o  -nostartfiles  -mcpu=cortex-a7 
LIBS:=#-L/home/helei/work/A31s/linux/lichee/buildroot/output/external-toolchain/lib/gcc/arm-linux-gnueabi/4.6.3  #  -lgcc_pic -lgcc_eh -lgcov


INC_TOP=$(shell pwd)

INCLUDEDIR 	:= -Iinclude -Ilib/libc
CFLAGS 		:= -Wall -O2 -fno-builtin 
CPPFLAGS   	:= -nostdinc  $(INCLUDEDIR)

export SDKROOT	CC AR LD OBJCOPY OBJDUMP INCLUDEDIR CFLAGS CPPFLAGS 

.PHONY :  all 

all:boot.bin
	echo $(INC_TOP)
boot.bin:$(OBJS)
	@echo $(OBJS)
	${LD} -Tboot.lds -o boot.elf $^ $(LIBS)
	${OBJCOPY} -O binary -S boot.elf $@
#	${OBJDUMP} -D boot.elf > boot.dis
	./tool/mksunxiboot boot.bin boot.bin

%.o:%.c
	${CC} $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

%.o:%.S
	${CC} $(CPPFLAGS) $(CFLAGS) -c -o $@ $<
lib/libc.a:
	make -C lib
cpu/libint.a:
	make -C cpu

clean:
	rm -f *.bin *.elf *.dis *.o *~
	make -C cpu clean
	make -C lib clean

