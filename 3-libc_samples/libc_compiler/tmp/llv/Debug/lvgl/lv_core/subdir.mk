################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lvgl/lv_core/lv_group.c \
../lvgl/lv_core/lv_indev.c \
../lvgl/lv_core/lv_obj.c \
../lvgl/lv_core/lv_refr.c \
../lvgl/lv_core/lv_style.c \
../lvgl/lv_core/lv_vdb.c 

OBJS += \
./lvgl/lv_core/lv_group.o \
./lvgl/lv_core/lv_indev.o \
./lvgl/lv_core/lv_obj.o \
./lvgl/lv_core/lv_refr.o \
./lvgl/lv_core/lv_style.o \
./lvgl/lv_core/lv_vdb.o 

C_DEPS += \
./lvgl/lv_core/lv_group.d \
./lvgl/lv_core/lv_indev.d \
./lvgl/lv_core/lv_obj.d \
./lvgl/lv_core/lv_refr.d \
./lvgl/lv_core/lv_style.d \
./lvgl/lv_core/lv_vdb.d 


# Each subdirectory must supply rules for building sources it contributes
lvgl/lv_core/%.o: ../lvgl/lv_core/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"/home/kisvegabor/projects/littlevgl/eclipse-workspace/pc_simulator" -O0 -Og -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


