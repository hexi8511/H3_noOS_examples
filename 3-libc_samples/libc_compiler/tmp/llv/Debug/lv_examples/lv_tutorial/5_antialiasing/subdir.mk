################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lv_examples/lv_tutorial/5_antialiasing/apple_icon_alpha.c \
../lv_examples/lv_tutorial/5_antialiasing/apple_icon_chroma.c \
../lv_examples/lv_tutorial/5_antialiasing/lv_tutorial_antialiasing.c 

OBJS += \
./lv_examples/lv_tutorial/5_antialiasing/apple_icon_alpha.o \
./lv_examples/lv_tutorial/5_antialiasing/apple_icon_chroma.o \
./lv_examples/lv_tutorial/5_antialiasing/lv_tutorial_antialiasing.o 

C_DEPS += \
./lv_examples/lv_tutorial/5_antialiasing/apple_icon_alpha.d \
./lv_examples/lv_tutorial/5_antialiasing/apple_icon_chroma.d \
./lv_examples/lv_tutorial/5_antialiasing/lv_tutorial_antialiasing.d 


# Each subdirectory must supply rules for building sources it contributes
lv_examples/lv_tutorial/5_antialiasing/%.o: ../lv_examples/lv_tutorial/5_antialiasing/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"/home/kisvegabor/projects/littlevgl/eclipse-workspace/pc_simulator" -O0 -Og -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


