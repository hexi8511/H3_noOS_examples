OBJS:=  lib1funcs.o\
		ctype.o\
		muldi3.o\
		printf.o\
		string.o\
		vsprintf.o\
		_udivsi3.o\
		div0.o\
		_divsi3.o\
		hang.o\
		div64.o
#OBJS:= 	_udivsi3.o\
		div0.o\
		_divsi3.o\
		hang.o\
		div64.o

INC_TOP	=$(shell pwd)	
INCLUDEDIR 	:= -I./include

CPPFLAGS   	= -nostdinc $(INCLUDEDIR)

libc.a: $(OBJS)
	echo $(INC_TOP)
	${AR} -r -o $@ $^
	
%.o:%.c
	${CC} $(CPPFLAGS) $(CFLAGS) -c -o   $@ $<

%.o:%.S
	${CC} $(CPPFLAGS) $(CFLAGS) -c -o  $@ $<

clean:
	rm -f libc.a *.o		
	
