SRC_C:=fifo.c\
		queue.c\
		rbtree.c\
	
		
	
SRC_S:=


OBJS:=$(patsubst %.c,%.o,$(wildcard *.c))
OBJS+=$(patsubst %.S,%.o,$(wildcard *.S))

		

INC_TOP	:=$(shell pwd)	
INCLUDEDIR 	:= -I../inc -I../../include/arch -I../include/asm

CPPFLAGS   := $(INCLUDEDIR)
#-nostdinc 

libx.a: $(OBJS)
	echo "================"
	echo  $(OBJS)
	#echo $(INC_TOP)
	${AR}  -c -r $@ $^
	
out/%.o:%.c
	${CC} $(CPPFLAGS) $(CFLAGS) -c  $^

out/%.o:%.S
	${CC} $(CPPFLAGS) $(CFLAGS) -c  $^

clean:
	rm -f libx.a *.o		
	
