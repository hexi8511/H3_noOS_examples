################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lv_examples/lv_tests/lv_test_objx/lv_test_roller/lv_test_roller.c 

OBJS += \
./lv_examples/lv_tests/lv_test_objx/lv_test_roller/lv_test_roller.o 

C_DEPS += \
./lv_examples/lv_tests/lv_test_objx/lv_test_roller/lv_test_roller.d 


# Each subdirectory must supply rules for building sources it contributes
lv_examples/lv_tests/lv_test_objx/lv_test_roller/%.o: ../lv_examples/lv_tests/lv_test_objx/lv_test_roller/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"/home/kisvegabor/projects/littlevgl/eclipse-workspace/pc_simulator" -O0 -Og -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

