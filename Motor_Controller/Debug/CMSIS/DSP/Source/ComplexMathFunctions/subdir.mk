################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CMSIS/DSP/Source/ComplexMathFunctions/ComplexMathFunctions.c 

OBJS += \
./CMSIS/DSP/Source/ComplexMathFunctions/ComplexMathFunctions.o 

C_DEPS += \
./CMSIS/DSP/Source/ComplexMathFunctions/ComplexMathFunctions.d 


# Each subdirectory must supply rules for building sources it contributes
CMSIS/DSP/Source/ComplexMathFunctions/%.o: ../CMSIS/DSP/Source/ComplexMathFunctions/%.c CMSIS/DSP/Source/ComplexMathFunctions/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MK64FN1M0VLL12 -DCPU_MK64FN1M0VLL12_cm4 -DSDK_OS_BAREMETAL -DSERIAL_PORT_TYPE_UART=1 -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\jonlo\Documents\MCUXpressoIDE_11.5.1_7266\workspace\Motor_Controller\drivers" -I"C:\Users\jonlo\Documents\MCUXpressoIDE_11.5.1_7266\workspace\Motor_Controller\utilities" -I"C:\Users\jonlo\Documents\MCUXpressoIDE_11.5.1_7266\workspace\Motor_Controller\component\serial_manager" -I"C:\Users\jonlo\Documents\MCUXpressoIDE_11.5.1_7266\workspace\Motor_Controller\component\uart" -I"C:\Users\jonlo\Documents\MCUXpressoIDE_11.5.1_7266\workspace\Motor_Controller\component\lists" -I"C:\Users\jonlo\Documents\MCUXpressoIDE_11.5.1_7266\workspace\Motor_Controller\CMSIS\DSP\Include" -I"C:\Users\jonlo\Documents\MCUXpressoIDE_11.5.1_7266\workspace\Motor_Controller\CMSIS" -I"C:\Users\jonlo\Documents\MCUXpressoIDE_11.5.1_7266\workspace\Motor_Controller\device" -I"C:\Users\jonlo\Documents\MCUXpressoIDE_11.5.1_7266\workspace\Motor_Controller\board" -I"C:\Users\jonlo\Documents\MCUXpressoIDE_11.5.1_7266\workspace\Motor_Controller\source" -I"C:\Users\jonlo\Documents\MCUXpressoIDE_11.5.1_7266\workspace\Motor_Controller\drivers" -I"C:\Users\jonlo\Documents\MCUXpressoIDE_11.5.1_7266\workspace\Motor_Controller\utilities" -I"C:\Users\jonlo\Documents\MCUXpressoIDE_11.5.1_7266\workspace\Motor_Controller\component\serial_manager" -I"C:\Users\jonlo\Documents\MCUXpressoIDE_11.5.1_7266\workspace\Motor_Controller\component\uart" -I"C:\Users\jonlo\Documents\MCUXpressoIDE_11.5.1_7266\workspace\Motor_Controller\component\lists" -I"C:\Users\jonlo\Documents\MCUXpressoIDE_11.5.1_7266\workspace\Motor_Controller\CMSIS" -I"C:\Users\jonlo\Documents\MCUXpressoIDE_11.5.1_7266\workspace\Motor_Controller\device" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


