SRCS:=lib1funcs.S\
		ctype.c\
		muldi3.c\
		printf.c\
		string.c\
		vsprintf.c\
		_udivsi3.S\
		div0.c\
		_divsi3.S\
		hang.c\
		div64.S




OBJS :=$(patsubst %.S, $(BUILD)/%.o, $(SRCS)) 
OBJS :=$(patsubst %.c,$(BUILD)/%.o, $(OBJS)) 

TARGET:=libc.a
 
INC_TOP	=$(shell pwd)	
INCLUDEDIR 	:= -I./libc

CPPFLAGS   	= -nostdinc $(INCLUDEDIR)

$(TARGET): $(OBJS)
	echo $(INC_TOP)
	${AR} -r -o $(BUILD)/$@ $^
	
$(BUILD)/%.o:%.c
	${CC} $(CPPFLAGS) $(CFLAGS) -c -o   $@ $<

$(BUILD)/%.o:%.S
	${CC} $(CPPFLAGS) $(CFLAGS) -c -o  $@ $<

clean:
	rm -f libc.a *.o		
	
