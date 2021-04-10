OBJS:=  cache.o\
	mmu_asm.o\
		mmu.o

INC_TOP	=$(shell pwd)	
INCLUDEDIR 	:= -I./\
				-I ../include

CPPFLAGS   	= -nostdinc $(INCLUDEDIR)

libmmu.a: $(OBJS)
	echo $(INC_TOP) ************
	${AR} -r -o $@ $^
	
%.o:%.c
	${CC} $(CPPFLAGS) $(CFLAGS) -c -o   $@ $<

%.o:%.S
	${CC} $(CPPFLAGS) $(CFLAGS) -c -o  $@ $<

clean:
	rm -f libmmu.a *.o		
	
