
SRCS:=	uart.c\
	delay.c\
	led.c\
	syscall.c\
	asm_test.c\
	main.c



OBJS :=$(patsubst %.S, $(BUILD)/%.o, $(SRCS)) 
OBJS :=$(patsubst %.c,$(BUILD)/%.o, $(OBJS)) 

TARGET=boot.bin
LDSCRIPT:=h3_dram.ld

#BUILD_OBJS =$(patsubst %.o, $(BUILD)/%.o, $(OBJS))  
#BUILD_OBJS +=$(patsubst %.a, $(BUILD)/%.a, $(OBJS))  



#CFLAGS:= -c  -marm -mcpu=cortex-a7  -  -O0 -g -Wall -nostdlib -marm 
# 
#LKFLAGS:= -o  -nostartfiles  -mcpu=cortex-a7 
#LIBS:=-L$(BUILD) $(BUILD)/libc.a $(BUILD)/libint.a $(BUILD)/libpll_data.a
LIBS:=
#-L$(BUILD)  -lcpu 
#
#-lpll_data
#-L/home/helei/work/A31s/linux/lichee/buildroot/output/external-toolchain/lib/gcc/arm-linux-gnueabi/4.6.3  # -lgcc -lgcc_pic -lgcc_eh -lgcov



INCLUDEDIR 	+=
CFLAGS 		+=
CPPFLAGS   	+= 



all: boot.bin
	echo $(INC_TOP)


$(TARGET):$(OBJS)
#	@echo $(OBJS)
	${CC} -T$(INC_TOP)/h3_dram.ld  $^  $(LIBS)  -nostartfiles 
#	${CC} -T$(INC_TOP)/h3_dram.ld -o $(INC_TOP)/boot.elf  $^  $(LIBS)  -nostartfiles 
#	${CC} -o $(INC_TOP)/boot.elf $^ $(LIBS) -g -T $(INC_TOP)/$(LDSCRIPT)   -nostartfiles -Wl,--gc-sections  -specs=rdimon.specs
	${OBJCOPY} -O binary -S $(INC_TOP)/boot.elf $(INC_TOP)/$@
#	${OBJDUMP} -D $(INC_TOP)/boot.elf > $(INC_TOP)/boot.dis
#	$(INC_TOP)/tool/mksunxiboot $(INC_TOP)/boot.bin $(INC_TOP)/boot.bin


$(BUILD)/%.o:%.S
	${CC} $(CPPFLAGS) $(CFLAGS) -c -o  $@ $<

$(BUILD)/%.o:%.c
	${CC} $(CPPFLAGS) $(CFLAGS) -c -o  $@ $<


clean:
	rm -f *.bin *.elf *.dis *.o *~
	make -C cpu clean
	make -C lib clean
	make -C pll_data clean

