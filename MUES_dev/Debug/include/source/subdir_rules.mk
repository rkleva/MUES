################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
include/source/%.obj: ../include/source/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1280/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -O2 --fp_mode=relaxed --include_path="C:/Users/Rio/Documents/Diplomski - 5. godina/Mikroprocesorko upavljanje električnim strojevima/Laboratorijske vježbe/Materijali/MUES_dev/MUES_dev" --include_path="C:/Users/Rio/Documents/Diplomski - 5. godina/Mikroprocesorko upavljanje električnim strojevima/Laboratorijske vježbe/Materijali/MUES_dev/MUES_dev/include/source" --include_path="C:/Users/Rio/Documents/Diplomski - 5. godina/Mikroprocesorko upavljanje električnim strojevima/Laboratorijske vježbe/Materijali/MUES_dev/MUES_dev/include/headers" --include_path="C:/Users/Rio/Documents/Diplomski - 5. godina/Mikroprocesorko upavljanje električnim strojevima/Laboratorijske vježbe/Materijali/MUES_dev/MUES_dev/include/common" --include_path="C:/ti/ccs1280/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/include" --define=CPU1 --define=_LAUNCHXL_F28379D -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="include/source/$(basename $(<F)).d_raw" --obj_directory="include/source" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

include/source/%.obj: ../include/source/%.asm $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1280/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -O2 --fp_mode=relaxed --include_path="C:/Users/Rio/Documents/Diplomski - 5. godina/Mikroprocesorko upavljanje električnim strojevima/Laboratorijske vježbe/Materijali/MUES_dev/MUES_dev" --include_path="C:/Users/Rio/Documents/Diplomski - 5. godina/Mikroprocesorko upavljanje električnim strojevima/Laboratorijske vježbe/Materijali/MUES_dev/MUES_dev/include/source" --include_path="C:/Users/Rio/Documents/Diplomski - 5. godina/Mikroprocesorko upavljanje električnim strojevima/Laboratorijske vježbe/Materijali/MUES_dev/MUES_dev/include/headers" --include_path="C:/Users/Rio/Documents/Diplomski - 5. godina/Mikroprocesorko upavljanje električnim strojevima/Laboratorijske vježbe/Materijali/MUES_dev/MUES_dev/include/common" --include_path="C:/ti/ccs1280/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/include" --define=CPU1 --define=_LAUNCHXL_F28379D -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="include/source/$(basename $(<F)).d_raw" --obj_directory="include/source" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


