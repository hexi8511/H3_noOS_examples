
SRCS:=	start.S\
	nano_lib.c\
	main.c

OBJS :=$(patsubst %.S, $(BUILD)/%.o, $(SRCS)) 
OBJS :=$(patsubst %.c,$(BUILD)/%.o, $(OBJS)) 

TARGET=boot0.bin


#BUILD_OBJS =$(patsubst %.o, $(BUILD)/%.o, $(OBJS))  
#BUILD_OBJS +=$(patsubst %.a, $(BUILD)/%.a, $(OBJS))  



#CFLAGS:= -c  -marm -mcpu=cortex-a7  -  -O0 -g -Wall -nostdlib -marm 
# 
#LKFLAGS:= -o  -nostartfiles  -mcpu=cortex-a7 
#LIBS:=-L$(BUILD) $(BUILD)/libc.a $(BUILD)/libint.a $(BUILD)/libpll_data.a
LIBS:=-L$(BUILD) -lcpu -lmmc -lpll_data -lbsp
#-lpll_data
#-L/home/helei/work/A31s/linux/lichee/buildroot/output/external-toolchain/lib/gcc/arm-linux-gnueabi/4.6.3  # -lgcc -lgcc_pic -lgcc_eh -lgcov



INCLUDEDIR 	+=
CFLAGS 		+=
CPPFLAGS   	+= 



all: boot0.bin
	echo $(INC_TOP)


$(TARGET):$(OBJS)
#	@echo $(OBJS)
	${LD} -Tboot.lds -o $(INC_TOP)/boot0.elf  $^  $(LIBS)
	${OBJCOPY} -O binary -S $(INC_TOP)/boot0.elf $(INC_TOP)/$@
#	${OBJDUMP} -D $(INC_TOP)/boot.elf > $(INC_TOP)/boot.dis
	$(INC_TOP)/tool/mksunxiboot $(INC_TOP)/boot0.bin $(INC_TOP)/boot0.bin


$(BUILD)/%.o:%.S
	${CC} $(CPPFLAGS) $(CFLAGS) -c -o  $@ $<

$(BUILD)/%.o:%.c
	${CC} $(CPPFLAGS) $(CFLAGS) -c -o  $@ $<


clean:
	rm -f *.bin *.elf *.dis *.o *~
	make -C cpu clean
	make -C lib clean
	make -C pll_data clean

