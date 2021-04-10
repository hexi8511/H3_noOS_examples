################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include lvgl/lv_themes/subdir.mk
-include lvgl/lv_objx/subdir.mk
-include lvgl/lv_misc/lv_fonts/subdir.mk
-include lvgl/lv_misc/subdir.mk
-include lvgl/lv_hal/subdir.mk
-include lvgl/lv_draw/subdir.mk
-include lvgl/lv_core/subdir.mk
-include lv_examples/lv_tutorial/9_responsive/subdir.mk
-include lv_examples/lv_tutorial/8_animations/subdir.mk
-include lv_examples/lv_tutorial/7_fonts/subdir.mk
-include lv_examples/lv_tutorial/6_images/subdir.mk
-include lv_examples/lv_tutorial/5_antialiasing/subdir.mk
-include lv_examples/lv_tutorial/4_themes/subdir.mk
-include lv_examples/lv_tutorial/3_styles/subdir.mk
-include lv_examples/lv_tutorial/2_objects/subdir.mk
-include lv_examples/lv_tutorial/1_hello_world/subdir.mk
-include lv_examples/lv_tutorial/10_keyboard/subdir.mk
-include lv_examples/lv_tutorial/0_porting/subdir.mk
-include lv_examples/lv_tests/lv_test_theme/subdir.mk
-include lv_examples/lv_tests/lv_test_stress/subdir.mk
-include lv_examples/lv_tests/lv_test_objx/lv_test_win/subdir.mk
-include lv_examples/lv_tests/lv_test_objx/lv_test_tabview/subdir.mk
-include lv_examples/lv_tests/lv_test_objx/lv_test_ta/subdir.mk
-include lv_examples/lv_tests/lv_test_objx/lv_test_sw/subdir.mk
-include lv_examples/lv_tests/lv_test_objx/lv_test_slider/subdir.mk
-include lv_examples/lv_tests/lv_test_objx/lv_test_roller/subdir.mk
-include lv_examples/lv_tests/lv_test_objx/lv_test_page/subdir.mk
-include lv_examples/lv_tests/lv_test_objx/lv_test_mbox/subdir.mk
-include lv_examples/lv_tests/lv_test_objx/lv_test_lmeter/subdir.mk
-include lv_examples/lv_tests/lv_test_objx/lv_test_list/subdir.mk
-include lv_examples/lv_tests/lv_test_objx/lv_test_line/subdir.mk
-include lv_examples/lv_tests/lv_test_objx/lv_test_led/subdir.mk
-include lv_examples/lv_tests/lv_test_objx/lv_test_label/subdir.mk
-include lv_examples/lv_tests/lv_test_objx/lv_test_kb/subdir.mk
-include lv_examples/lv_tests/lv_test_objx/lv_test_img/subdir.mk
-include lv_examples/lv_tests/lv_test_objx/lv_test_gauge/subdir.mk
-include lv_examples/lv_tests/lv_test_objx/lv_test_ddlist/subdir.mk
-include lv_examples/lv_tests/lv_test_objx/lv_test_cont/subdir.mk
-include lv_examples/lv_tests/lv_test_objx/lv_test_chart/subdir.mk
-include lv_examples/lv_tests/lv_test_objx/lv_test_cb/subdir.mk
-include lv_examples/lv_tests/lv_test_objx/lv_test_btnm/subdir.mk
-include lv_examples/lv_tests/lv_test_objx/lv_test_btn/subdir.mk
-include lv_examples/lv_tests/lv_test_objx/lv_test_bar/subdir.mk
-include lv_examples/lv_tests/lv_test_obj/subdir.mk
-include lv_examples/lv_tests/lv_test_misc/subdir.mk
-include lv_examples/lv_tests/lv_test_group/subdir.mk
-include lv_examples/lv_apps/tpcal/subdir.mk
-include lv_examples/lv_apps/terminal/subdir.mk
-include lv_examples/lv_apps/sysmon/subdir.mk
-include lv_examples/lv_apps/demo/subdir.mk
-include lv_examples/lv_apps/benchmark/subdir.mk
-include lv_drivers/indev/subdir.mk
-include lv_drivers/display/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: pc_simulator

# Tool invocations
pc_simulator: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross GCC Linker'
	gcc  -o "pc_simulator" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) pc_simulator
	-@echo ' '

.PHONY: all clean dependents

-include ../makefile.targets
