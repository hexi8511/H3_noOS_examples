SRC_C:=$(wildcard ./*.c)  
SRC_C+=$(wildcard ./arm32/*.c)  
	
SRC_S:=

OBJS:=$(patsubst %.c,%.o,$(wildcard *.c))
OBJS+=$(patsubst %.S,%.o,$(wildcard *.S))

		

INC_TOP	:=$(shell pwd)	
INCLUDEDIR 	:= -I../inc -I../../include/arch -I../include/asm

CPPFLAGS   := $(INCLUDEDIR) -nostdinc 

libm.a: $(OBJS)
	echo "================"
	echo  $(OBJS)
	#echo $(INC_TOP)
	${AR}  -c -r $@ $^
	
out/%.o:%.c
	${CC} $(CPPFLAGS) $(CFLAGS) -c  $^

out/%.o:%.S
	${CC} $(CPPFLAGS) $(CFLAGS) -c  $^

clean:
	rm -f libm.a *.o		
	
