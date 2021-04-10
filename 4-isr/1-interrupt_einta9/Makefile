
CC      = arm-none-eabi-gcc
LD      = arm-none-eabi-ld
AR      = arm-none-eabi-ar
OBJCOPY = arm-none-eabi-objcopy
BJDUMP = arm-none-eabi-objdump


BUILD:=$(shell pwd)/build

INCLUDE:=$(shell pwd)/include

INC_TOP=$(shell pwd)

INCLUDEDIR 	:= -I$(INCLUDE) -I$(INCLUDE)/libc

CFLAGS 		:= -Wall -O2 -fno-builtin -nostdinc  $(INCLUDEDIR)
CPPFLAGS   	:= 

export SDKROOT CC AR LD OBJCOPY OBJDUMP INCLUDEDIR CFLAGS CPPFLAGS  BUILD INC_TOP



#debug文件夹里的makefile文件需要最后执行，所以这里需要执行的子目录要排除debug文件夹，这里使用awk排除了debug文件夹，读取剩下的文件夹
SUBDIRS=$(shell ls -l | grep ^d | awk '{if($$9 != "debug") print $$9}')

#SUBDIRS删除includes文件夹，因为这个文件中是头文件，不需要make
SUBDIRS:=$(patsubst include,,$(SUBDIRS))
#SUBDIRS:=$(patsubst out,,$(SUBDIRS))
SUBDIRS:=$(patsubst tool,,$(SUBDIRS))
SUBDIRS:=$(patsubst build,,$(SUBDIRS))

CLEAN_SUBDIRS:=$(SUBDIRS)

#.PHONY:  all  clean
##$@ all target files. $^ all depened files. $< the first depened file

all: init $(SUBDIRS)
	@echo "===========Start compile project======="
	@echo $^

$(SUBDIRS):ECHO
	@echo "===========Start compile libs======="
	$(MAKE) -C $@
ECHO:
	@echo $(SUBDIRS)

init:
	if [ ! -e $(BUILD) ];then \
	mkdir $(BUILD);fi


clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c *.a *.elf *.bin
	@echo "===========Start clean libs======="
	rm -rf $(BUILD)/*

