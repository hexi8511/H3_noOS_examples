
SRCS:=\
	arm_start.S\
	cache.S\
	intc.c\
	int.S\
	cp15_v.c\
	mmu.c\
	mmu_asm.S\
	dump_cpu_regs.S


OBJS :=$(patsubst %.S, $(BUILD)/%.o, $(SRCS)) 
OBJS :=$(patsubst %.c,$(BUILD)/%.o, $(OBJS)) 

TARGET:=libcpu.a

INC_TOP	=$(shell pwd)	
INCLUDEDIR 	:= -I./\
		   -I ../include

CPPFLAGS   	= -nostdinc $(INCLUDEDIR)

$(TARGET):$(OBJS)
	echo $(INC_TOP) ************
	${AR} -r -o $(BUILD)/$@ $^
	
$(BUILD)/%.o:%.c
	${CC} $(CPPFLAGS) $(CFLAGS) -c -o  $@ $<

$(BUILD)/%.o:%.S
	${CC} $(CPPFLAGS) $(CFLAGS) -c -o  $@ $<

clean:
	rm -f libint.a *.o		
	
