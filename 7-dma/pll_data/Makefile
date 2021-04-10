SRCS:=audio_pll_data.c\
			cpux_pll_data.c\
			ddr_pll_data.c\
			periphx_pll_data.c\
			ve_pll_data.c

OBJS :=$(patsubst %.S, $(BUILD)/%.o, $(SRCS)) 
OBJS :=$(patsubst %.c,$(BUILD)/%.o, $(OBJS)) 

TARGET=libpll_data.a

INC_TOP	=$(shell pwd)	
INCLUDEDIR 	:= -I./\
				-I ../include

CPPFLAGS   	= -nostdinc $(INCLUDEDIR)

$(TARGET): $(OBJS)
	echo $(INC_TOP) ************
	${AR} -r -o $(BUILD)/$@ $^
	
$(BUILD)/%.o:%.c
	${CC} $(CPPFLAGS) $(CFLAGS) -c -o   $@ $<

$(BUILD)/%.o:%.S
	${CC} $(CPPFLAGS) $(CFLAGS) -c -o  $@ $<

clean:
	rm -f libpll_data.a *.o		
	
