################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lv_examples/lv_tutorial/1_hello_world/lv_tutorial_hello_world.c 

OBJS += \
./lv_examples/lv_tutorial/1_hello_world/lv_tutorial_hello_world.o 

C_DEPS += \
./lv_examples/lv_tutorial/1_hello_world/lv_tutorial_hello_world.d 


# Each subdirectory must supply rules for building sources it contributes
lv_examples/lv_tutorial/1_hello_world/%.o: ../lv_examples/lv_tutorial/1_hello_world/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"/home/kisvegabor/projects/littlevgl/eclipse-workspace/pc_simulator" -O0 -Og -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


