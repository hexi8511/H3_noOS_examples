################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lvgl/lv_hal/lv_hal_disp.c \
../lvgl/lv_hal/lv_hal_indev.c \
../lvgl/lv_hal/lv_hal_tick.c 

OBJS += \
./lvgl/lv_hal/lv_hal_disp.o \
./lvgl/lv_hal/lv_hal_indev.o \
./lvgl/lv_hal/lv_hal_tick.o 

C_DEPS += \
./lvgl/lv_hal/lv_hal_disp.d \
./lvgl/lv_hal/lv_hal_indev.d \
./lvgl/lv_hal/lv_hal_tick.d 


# Each subdirectory must supply rules for building sources it contributes
lvgl/lv_hal/%.o: ../lvgl/lv_hal/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"/home/kisvegabor/projects/littlevgl/eclipse-workspace/pc_simulator" -O0 -Og -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


