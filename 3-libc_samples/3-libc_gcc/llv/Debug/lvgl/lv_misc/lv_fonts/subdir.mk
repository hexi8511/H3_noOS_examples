################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lvgl/lv_misc/lv_fonts/lv_font_dejavu_10.c \
../lvgl/lv_misc/lv_fonts/lv_font_dejavu_10_cyrillic.c \
../lvgl/lv_misc/lv_fonts/lv_font_dejavu_10_latin_sup.c \
../lvgl/lv_misc/lv_fonts/lv_font_dejavu_20.c \
../lvgl/lv_misc/lv_fonts/lv_font_dejavu_20_cyrillic.c \
../lvgl/lv_misc/lv_fonts/lv_font_dejavu_20_latin_sup.c \
../lvgl/lv_misc/lv_fonts/lv_font_dejavu_30.c \
../lvgl/lv_misc/lv_fonts/lv_font_dejavu_30_cyrillic.c \
../lvgl/lv_misc/lv_fonts/lv_font_dejavu_30_latin_sup.c \
../lvgl/lv_misc/lv_fonts/lv_font_dejavu_40.c \
../lvgl/lv_misc/lv_fonts/lv_font_dejavu_40_cyrillic.c \
../lvgl/lv_misc/lv_fonts/lv_font_dejavu_40_latin_sup.c \
../lvgl/lv_misc/lv_fonts/lv_font_symbol_10.c \
../lvgl/lv_misc/lv_fonts/lv_font_symbol_20.c \
../lvgl/lv_misc/lv_fonts/lv_font_symbol_30.c \
../lvgl/lv_misc/lv_fonts/lv_font_symbol_40.c 

OBJS += \
./lvgl/lv_misc/lv_fonts/lv_font_dejavu_10.o \
./lvgl/lv_misc/lv_fonts/lv_font_dejavu_10_cyrillic.o \
./lvgl/lv_misc/lv_fonts/lv_font_dejavu_10_latin_sup.o \
./lvgl/lv_misc/lv_fonts/lv_font_dejavu_20.o \
./lvgl/lv_misc/lv_fonts/lv_font_dejavu_20_cyrillic.o \
./lvgl/lv_misc/lv_fonts/lv_font_dejavu_20_latin_sup.o \
./lvgl/lv_misc/lv_fonts/lv_font_dejavu_30.o \
./lvgl/lv_misc/lv_fonts/lv_font_dejavu_30_cyrillic.o \
./lvgl/lv_misc/lv_fonts/lv_font_dejavu_30_latin_sup.o \
./lvgl/lv_misc/lv_fonts/lv_font_dejavu_40.o \
./lvgl/lv_misc/lv_fonts/lv_font_dejavu_40_cyrillic.o \
./lvgl/lv_misc/lv_fonts/lv_font_dejavu_40_latin_sup.o \
./lvgl/lv_misc/lv_fonts/lv_font_symbol_10.o \
./lvgl/lv_misc/lv_fonts/lv_font_symbol_20.o \
./lvgl/lv_misc/lv_fonts/lv_font_symbol_30.o \
./lvgl/lv_misc/lv_fonts/lv_font_symbol_40.o 

C_DEPS += \
./lvgl/lv_misc/lv_fonts/lv_font_dejavu_10.d \
./lvgl/lv_misc/lv_fonts/lv_font_dejavu_10_cyrillic.d \
./lvgl/lv_misc/lv_fonts/lv_font_dejavu_10_latin_sup.d \
./lvgl/lv_misc/lv_fonts/lv_font_dejavu_20.d \
./lvgl/lv_misc/lv_fonts/lv_font_dejavu_20_cyrillic.d \
./lvgl/lv_misc/lv_fonts/lv_font_dejavu_20_latin_sup.d \
./lvgl/lv_misc/lv_fonts/lv_font_dejavu_30.d \
./lvgl/lv_misc/lv_fonts/lv_font_dejavu_30_cyrillic.d \
./lvgl/lv_misc/lv_fonts/lv_font_dejavu_30_latin_sup.d \
./lvgl/lv_misc/lv_fonts/lv_font_dejavu_40.d \
./lvgl/lv_misc/lv_fonts/lv_font_dejavu_40_cyrillic.d \
./lvgl/lv_misc/lv_fonts/lv_font_dejavu_40_latin_sup.d \
./lvgl/lv_misc/lv_fonts/lv_font_symbol_10.d \
./lvgl/lv_misc/lv_fonts/lv_font_symbol_20.d \
./lvgl/lv_misc/lv_fonts/lv_font_symbol_30.d \
./lvgl/lv_misc/lv_fonts/lv_font_symbol_40.d 


# Each subdirectory must supply rules for building sources it contributes
lvgl/lv_misc/lv_fonts/%.o: ../lvgl/lv_misc/lv_fonts/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"/home/kisvegabor/projects/littlevgl/eclipse-workspace/pc_simulator" -O0 -Og -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


