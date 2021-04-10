SRCS:=start.S\
	arm_start.S\
	int.S\
	cache.S

OBJS =$(patsubst %.S, $(BUILD)/%.o, $(SRCS))  


TARGET:=libint.a

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
	
