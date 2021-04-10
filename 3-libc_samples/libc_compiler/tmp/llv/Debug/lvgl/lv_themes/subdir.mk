################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lvgl/lv_themes/lv_theme.c \
../lvgl/lv_themes/lv_theme_alien.c \
../lvgl/lv_themes/lv_theme_default.c \
../lvgl/lv_themes/lv_theme_material.c \
../lvgl/lv_themes/lv_theme_mono.c \
../lvgl/lv_themes/lv_theme_night.c \
../lvgl/lv_themes/lv_theme_templ.c \
../lvgl/lv_themes/lv_theme_zen.c 

OBJS += \
./lvgl/lv_themes/lv_theme.o \
./lvgl/lv_themes/lv_theme_alien.o \
./lvgl/lv_themes/lv_theme_default.o \
./lvgl/lv_themes/lv_theme_material.o \
./lvgl/lv_themes/lv_theme_mono.o \
./lvgl/lv_themes/lv_theme_night.o \
./lvgl/lv_themes/lv_theme_templ.o \
./lvgl/lv_themes/lv_theme_zen.o 

C_DEPS += \
./lvgl/lv_themes/lv_theme.d \
./lvgl/lv_themes/lv_theme_alien.d \
./lvgl/lv_themes/lv_theme_default.d \
./lvgl/lv_themes/lv_theme_material.d \
./lvgl/lv_themes/lv_theme_mono.d \
./lvgl/lv_themes/lv_theme_night.d \
./lvgl/lv_themes/lv_theme_templ.d \
./lvgl/lv_themes/lv_theme_zen.d 


# Each subdirectory must supply rules for building sources it contributes
lvgl/lv_themes/%.o: ../lvgl/lv_themes/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"/home/kisvegabor/projects/littlevgl/eclipse-workspace/pc_simulator" -O0 -Og -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


