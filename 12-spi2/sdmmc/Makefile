SRCS:=	mmc_bsp.c\
	mmc.c\
	mmc_op.c\
	load_boot1_from_sdmmc.c

OBJS =$(patsubst %.S, $(BUILD)/%.o, $(SRCS))  
OBJS :=$(patsubst %.c,$(BUILD)/%.o, $(OBJS)) 

TARGET:=libmmc.a

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
	
