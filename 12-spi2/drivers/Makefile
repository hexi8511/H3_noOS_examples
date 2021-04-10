SRCS:=	uart.c\
	dma.c\
	spi.c\
	delay.c\
	ccu.c\
	timer.c\
	led.c\
	rader.c\
	rtc.c\
	power.c\
	dram_sun8i_h3.c\
	wdog.c
			

OBJS =$(patsubst %.S, $(BUILD)/%.o, $(SRCS))  
OBJS :=$(patsubst %.c,$(BUILD)/%.o, $(OBJS)) 

TARGET:=libbsp.a

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
	
