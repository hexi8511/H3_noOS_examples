OBJS:=  	audio_pll_data.o\
			cpux_pll_data.o\
			ddr_pll_data.o\
			periphx_pll_data.o\
			ve_pll_data.o

INC_TOP	=$(shell pwd)	
INCLUDEDIR 	:= -I./\
				-I ../include

CPPFLAGS   	= -nostdinc $(INCLUDEDIR)

libpll_data.a: $(OBJS)
	echo $(INC_TOP) ************
	${AR} -r -o $@ $^
	
%.o:%.c
	${CC} $(CPPFLAGS) $(CFLAGS) -c -o   $@ $<

%.o:%.S
	${CC} $(CPPFLAGS) $(CFLAGS) -c -o  $@ $<

clean:
	rm -f libpll_data.a *.o		
	
