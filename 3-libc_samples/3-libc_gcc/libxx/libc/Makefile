SRC_C:=ssize.c

SRC_C+=$(wildcard charset/*.c)
SRC_C+=$(wildcard crypto/*.c)
SRC_C+=$(wildcard ctype/*.c)
SRC_C+=$(wildcard environ/*.c)
SRC_C+=$(wildcard errno/*.c)
SRC_C+=$(wildcard exit/*.c)
SRC_C+=$(wildcard filter/*.c)
SRC_C+=$(wildcard locale/*.c)
SRC_C+=$(wildcard path/*.c)
SRC_C+=$(wildcard malloc/*.c)
SRC_C+=$(wildcard stdio/*.c)
SRC_C+=$(wildcard stdlib/*.c)
SRC_C+=$(wildcard string/*.c)
SRC_C+=$(wildcard time/*.c)



OBJS:=$(patsubst %.c,%.o,$(SRC_C))


		

INC_TOP	:=$(shell pwd)	
INCLUDEDIR 	:= -I../inc -I../../include/arch -I../include/asm

CPPFLAGS   := $(INCLUDEDIR) -nostdinc 

libc.a: $(OBJS)
	echo "================"
	echo  $(OBJS)
	echo "################"
	#echo $(INC_TOP)
	${AR}  -c -r $@ $^
	
out/%.o:%.c
	${CC} $(CPPFLAGS) $(CFLAGS) -c  $^

out/%.o:%.S
	${CC} $(CPPFLAGS) $(CFLAGS) -c  $^

clean:
	rm -f libc.a *.o		
	
