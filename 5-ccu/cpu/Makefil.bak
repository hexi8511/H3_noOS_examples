OBJS:=  arm_start.o\
		int.o

INC_TOP	=$(shell pwd)	
INCLUDEDIR 	:= -I./\
				-I ../include

CPPFLAGS   	= -nostdinc $(INCLUDEDIR)

libint.a: $(OBJS)
	echo $(INC_TOP) ************
	${AR} -r -o $@ $^
	
%.o:%.c
	${CC} $(CPPFLAGS) $(CFLAGS) -c -o   $@ $<

%.o:%.S
	${CC} $(CPPFLAGS) $(CFLAGS) -c -o  $@ $<

clean:
	rm -f libint.a *.o		
	
