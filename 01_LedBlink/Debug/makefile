################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Startup/subdir.mk
-include Src/subdir.mk
-include Drivers/STM32F4xx_HAL_Driver/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
01_LedBlink.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
01_LedBlink.list \

OBJCOPY_HEX += \
01_LedBlink.hex \

OBJCOPY_BIN += \
01_LedBlink.bin \


# All Target
all: 01_LedBlink.elf secondary-outputs

# Tool invocations
01_LedBlink.elf: $(OBJS) $(USER_OBJS) F:\File\STM32\stm32_exercise\Cube_IDE_Workspace\F429_Workspace\Stm32F429IGT6_Learning\01_LedBlink\STM32F429IGTX_FLASH.ld
	arm-none-eabi-gcc -o "01_LedBlink.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"F:\File\STM32\stm32_exercise\Cube_IDE_Workspace\F429_Workspace\Stm32F429IGT6_Learning\01_LedBlink\STM32F429IGTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="01_LedBlink.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

01_LedBlink.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "01_LedBlink.list"
	@echo 'Finished building: $@'
	@echo ' '

01_LedBlink.hex: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O ihex $(EXECUTABLES) "01_LedBlink.hex"
	@echo 'Finished building: $@'
	@echo ' '

01_LedBlink.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "01_LedBlink.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_HEX) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
