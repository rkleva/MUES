################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccs1280/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./MotorControl.obj" \
"./board_setup.obj" \
"./control_lib.obj" \
"./main.obj" \
"./include/source/DRV8301_SPI.obj" \
"./include/source/F2837xD_Adc.obj" \
"./include/source/F2837xD_CodeStartBranch.obj" \
"./include/source/F2837xD_DefaultISR.obj" \
"./include/source/F2837xD_EPwm.obj" \
"./include/source/F2837xD_GlobalVariableDefs.obj" \
"./include/source/F2837xD_Gpio.obj" \
"./include/source/F2837xD_Ipc.obj" \
"./include/source/F2837xD_PieCtrl.obj" \
"./include/source/F2837xD_PieVect.obj" \
"./include/source/F2837xD_SysCtrl.obj" \
"./include/source/F2837xD_usDelay.obj" \
"./include/source/dlog_4ch_f.obj" \
"../include/source/2837x_RAM_lnk_cpu1.cmd" \
"../include/source/F2837xD_Headers_nonBIOS_cpu1.cmd" \
$(GEN_CMDS__FLAG) \
-llibc.a \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include include/source/subdir_vars.mk
-include subdir_rules.mk
-include include/source/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
MUES_dev.out 

EXE_OUTPUTS__QUOTED += \
"MUES_dev.out" 


# All Target
all: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "MUES_dev.out"

# Tool invocations
MUES_dev.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: C2000 Linker'
	"C:/ti/ccs1280/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -O2 --fp_mode=relaxed --define=CPU1 --define=_LAUNCHXL_F28379D -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi -z -m"MUES_dev.map" --stack_size=0x200 --warn_sections -i"C:/ti/ccs1280/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/lib" -i"C:/ti/ccs1280/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/include" --reread_libs --diag_wrap=off --display_error_number --xml_link_info="MUES_dev_linkInfo.xml" --rom_model -o "MUES_dev.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)
	-$(RM) "MotorControl.obj" "board_setup.obj" "control_lib.obj" "main.obj" "include\source\DRV8301_SPI.obj" "include\source\F2837xD_Adc.obj" "include\source\F2837xD_CodeStartBranch.obj" "include\source\F2837xD_DefaultISR.obj" "include\source\F2837xD_EPwm.obj" "include\source\F2837xD_GlobalVariableDefs.obj" "include\source\F2837xD_Gpio.obj" "include\source\F2837xD_Ipc.obj" "include\source\F2837xD_PieCtrl.obj" "include\source\F2837xD_PieVect.obj" "include\source\F2837xD_SysCtrl.obj" "include\source\F2837xD_usDelay.obj" "include\source\dlog_4ch_f.obj" 
	-$(RM) "MotorControl.d" "board_setup.d" "control_lib.d" "main.d" "include\source\DRV8301_SPI.d" "include\source\F2837xD_Adc.d" "include\source\F2837xD_DefaultISR.d" "include\source\F2837xD_EPwm.d" "include\source\F2837xD_GlobalVariableDefs.d" "include\source\F2837xD_Gpio.d" "include\source\F2837xD_Ipc.d" "include\source\F2837xD_PieCtrl.d" "include\source\F2837xD_PieVect.d" "include\source\F2837xD_SysCtrl.d" "include\source\dlog_4ch_f.d" 
	-$(RM) "include\source\F2837xD_CodeStartBranch.d" "include\source\F2837xD_usDelay.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

