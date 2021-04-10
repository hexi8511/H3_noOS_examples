################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lv_examples/lv_tutorial/7_fonts/arial_ascii_20.c \
../lv_examples/lv_tutorial/7_fonts/arial_cyrillic_20.c \
../lv_examples/lv_tutorial/7_fonts/arial_math_20.c \
../lv_examples/lv_tutorial/7_fonts/lv_tutorial_fonts.c 

OBJS += \
./lv_examples/lv_tutorial/7_fonts/arial_ascii_20.o \
./lv_examples/lv_tutorial/7_fonts/arial_cyrillic_20.o \
./lv_examples/lv_tutorial/7_fonts/arial_math_20.o \
./lv_examples/lv_tutorial/7_fonts/lv_tutorial_fonts.o 

C_DEPS += \
./lv_examples/lv_tutorial/7_fonts/arial_ascii_20.d \
./lv_examples/lv_tutorial/7_fonts/arial_cyrillic_20.d \
./lv_examples/lv_tutorial/7_fonts/arial_math_20.d \
./lv_examples/lv_tutorial/7_fonts/lv_tutorial_fonts.d 


# Each subdirectory must supply rules for building sources it contributes
lv_examples/lv_tutorial/7_fonts/%.o: ../lv_examples/lv_tutorial/7_fonts/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"/home/kisvegabor/projects/littlevgl/eclipse-workspace/pc_simulator" -O0 -Og -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


