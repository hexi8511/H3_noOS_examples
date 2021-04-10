################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lvgl/lv_misc/lv_anim.c \
../lvgl/lv_misc/lv_area.c \
../lvgl/lv_misc/lv_circ.c \
../lvgl/lv_misc/lv_color.c \
../lvgl/lv_misc/lv_font.c \
../lvgl/lv_misc/lv_fs.c \
../lvgl/lv_misc/lv_ll.c \
../lvgl/lv_misc/lv_math.c \
../lvgl/lv_misc/lv_mem.c \
../lvgl/lv_misc/lv_task.c \
../lvgl/lv_misc/lv_templ.c \
../lvgl/lv_misc/lv_trigo.c \
../lvgl/lv_misc/lv_txt.c \
../lvgl/lv_misc/lv_ufs.c 

OBJS += \
./lvgl/lv_misc/lv_anim.o \
./lvgl/lv_misc/lv_area.o \
./lvgl/lv_misc/lv_circ.o \
./lvgl/lv_misc/lv_color.o \
./lvgl/lv_misc/lv_font.o \
./lvgl/lv_misc/lv_fs.o \
./lvgl/lv_misc/lv_ll.o \
./lvgl/lv_misc/lv_math.o \
./lvgl/lv_misc/lv_mem.o \
./lvgl/lv_misc/lv_task.o \
./lvgl/lv_misc/lv_templ.o \
./lvgl/lv_misc/lv_trigo.o \
./lvgl/lv_misc/lv_txt.o \
./lvgl/lv_misc/lv_ufs.o 

C_DEPS += \
./lvgl/lv_misc/lv_anim.d \
./lvgl/lv_misc/lv_area.d \
./lvgl/lv_misc/lv_circ.d \
./lvgl/lv_misc/lv_color.d \
./lvgl/lv_misc/lv_font.d \
./lvgl/lv_misc/lv_fs.d \
./lvgl/lv_misc/lv_ll.d \
./lvgl/lv_misc/lv_math.d \
./lvgl/lv_misc/lv_mem.d \
./lvgl/lv_misc/lv_task.d \
./lvgl/lv_misc/lv_templ.d \
./lvgl/lv_misc/lv_trigo.d \
./lvgl/lv_misc/lv_txt.d \
./lvgl/lv_misc/lv_ufs.d 


# Each subdirectory must supply rules for building sources it contributes
lvgl/lv_misc/%.o: ../lvgl/lv_misc/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"/home/kisvegabor/projects/littlevgl/eclipse-workspace/pc_simulator" -O0 -Og -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


