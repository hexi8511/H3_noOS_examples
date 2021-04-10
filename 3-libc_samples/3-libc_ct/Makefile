
CC      = arm-none-eabi-gcc
LD      = arm-none-eabi-ld
AR      = arm-none-eabi-ar
OBJCOPY = arm-none-eabi-objcopy
BJDUMP = arm-none-eabi-objdump


INCLUDE:=include

OBJS:=  lib/libc.a\
			start.o \
			uart.o\
			delay.o\
			led.o\
			main.o
#CFLAGS:= -c  -marm -mcpu=cortex-a7  -  -O0 -g -Wall -nostdlib -marm 
# 
#LKFLAGS:= -o  -nostartfiles  -mcpu=cortex-a7 
LIBS:=#-L/home/helei/work/A31s/linux/lichee/buildroot/output/external-toolchain/lib/gcc/arm-linux-gnueabi/4.6.3  #  -lgcc_pic -lgcc_eh -lgcov


INC_TOP=$(shell pwd)

INCLUDEDIR 	:= -Iinclude\
				-Ilib/libc\
				-Ilib
CFLAGS 		:= -Wall -O2 -fno-builtin 
CPPFLAGS   	:= -nostdinc  $(INCLUDEDIR) -Llib 

export SDKROOT	CC AR LD OBJCOPY OBJDUMP INCLUDEDIR CFLAGS CPPFLAGS 

TARGET=boot.bin

.PHONY :  all 

all:$(TARGET)
	echo $(INC_TOP)
$(TARGET):$(OBJS)  lib/libc.a
	@echo $(OBJS)
	${LD} -Tboot.lds -o boot.elf $^ $(LIBS)
	${OBJCOPY} -O binary -S boot.elf $@
#	${OBJDUMP} -D boot.elf > boot.dis
	./tool/mksunxiboot $(TARGET) $(TARGET)

%.o:%.c
	${CC} $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

%.o:%.S
	${CC} $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

lib/libc.a:
	make -C lib

clean:
	rm -f *.bin *.elf *.dis *.o *~
	make -C lib clean



