################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lv_examples/lv_apps/benchmark/benchmark.c \
../lv_examples/lv_apps/benchmark/benchmark_bg.c 

OBJS += \
./lv_examples/lv_apps/benchmark/benchmark.o \
./lv_examples/lv_apps/benchmark/benchmark_bg.o 

C_DEPS += \
./lv_examples/lv_apps/benchmark/benchmark.d \
./lv_examples/lv_apps/benchmark/benchmark_bg.d 


# Each subdirectory must supply rules for building sources it contributes
lv_examples/lv_apps/benchmark/%.o: ../lv_examples/lv_apps/benchmark/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"/home/kisvegabor/projects/littlevgl/eclipse-workspace/pc_simulator" -O0 -Og -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


