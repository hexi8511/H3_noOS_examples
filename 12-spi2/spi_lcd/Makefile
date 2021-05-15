SRCS:=	spi_lcd.c\
	family3.c\
	spi_gpio.c

OBJS =$(patsubst %.S, $(BUILD)/%.o, $(SRCS))  
OBJS :=$(patsubst %.c,$(BUILD)/%.o, $(OBJS)) 

TARGET:=liblcd.a 

INC_TOP	=$(shell pwd)	
INCLUDEDIR 	:= -I./\
		   -I ../include

CPPFLAGS   	= -nostdinc $(INCLUDEDIR)

LIBS:=-L$(BUILD) -lcpu -lc  -lbsp

$(TARGET):$(OBJS)
	echo $(INC_TOP) ************
	${AR} -r -o $(BUILD)/$@ $^
	
$(BUILD)/%.o:%.c
	${CC} $(CPPFLAGS) $(CFLAGS) -c -o  $@ $< $(LIBS)

$(BUILD)/%.o:%.S
	${CC} $(CPPFLAGS) $(CFLAGS) -c -o  $@ $< $(LIBS)

clean:
	rm -f libint.a *.o		
	
