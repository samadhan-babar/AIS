################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/Src/bsp/bsp_10_0.cpp \
../Core/Src/bsp/bsp_10_1.cpp \
../Core/Src/bsp/bsp_10_5.cpp \
../Core/Src/bsp/bsp_10_9.cpp \
../Core/Src/bsp/bsp_11_0.cpp \
../Core/Src/bsp/bsp_11_3.cpp \
../Core/Src/bsp/bsp_9_3.cpp 

OBJS += \
./Core/Src/bsp/bsp_10_0.o \
./Core/Src/bsp/bsp_10_1.o \
./Core/Src/bsp/bsp_10_5.o \
./Core/Src/bsp/bsp_10_9.o \
./Core/Src/bsp/bsp_11_0.o \
./Core/Src/bsp/bsp_11_3.o \
./Core/Src/bsp/bsp_9_3.o 

CPP_DEPS += \
./Core/Src/bsp/bsp_10_0.d \
./Core/Src/bsp/bsp_10_1.d \
./Core/Src/bsp/bsp_10_5.d \
./Core/Src/bsp/bsp_10_9.d \
./Core/Src/bsp/bsp_11_0.d \
./Core/Src/bsp/bsp_11_3.d \
./Core/Src/bsp/bsp_9_3.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/bsp/%.o Core/Src/bsp/%.su Core/Src/bsp/%.cyclo: ../Core/Src/bsp/%.cpp Core/Src/bsp/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L432xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O3 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-bsp

clean-Core-2f-Src-2f-bsp:
	-$(RM) ./Core/Src/bsp/bsp_10_0.cyclo ./Core/Src/bsp/bsp_10_0.d ./Core/Src/bsp/bsp_10_0.o ./Core/Src/bsp/bsp_10_0.su ./Core/Src/bsp/bsp_10_1.cyclo ./Core/Src/bsp/bsp_10_1.d ./Core/Src/bsp/bsp_10_1.o ./Core/Src/bsp/bsp_10_1.su ./Core/Src/bsp/bsp_10_5.cyclo ./Core/Src/bsp/bsp_10_5.d ./Core/Src/bsp/bsp_10_5.o ./Core/Src/bsp/bsp_10_5.su ./Core/Src/bsp/bsp_10_9.cyclo ./Core/Src/bsp/bsp_10_9.d ./Core/Src/bsp/bsp_10_9.o ./Core/Src/bsp/bsp_10_9.su ./Core/Src/bsp/bsp_11_0.cyclo ./Core/Src/bsp/bsp_11_0.d ./Core/Src/bsp/bsp_11_0.o ./Core/Src/bsp/bsp_11_0.su ./Core/Src/bsp/bsp_11_3.cyclo ./Core/Src/bsp/bsp_11_3.d ./Core/Src/bsp/bsp_11_3.o ./Core/Src/bsp/bsp_11_3.su ./Core/Src/bsp/bsp_9_3.cyclo ./Core/Src/bsp/bsp_9_3.d ./Core/Src/bsp/bsp_9_3.o ./Core/Src/bsp/bsp_9_3.su

.PHONY: clean-Core-2f-Src-2f-bsp

