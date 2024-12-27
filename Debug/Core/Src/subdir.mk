################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/Src/AISMessages.cpp \
../Core/Src/AODV_mesh.cpp \
../Core/Src/ChannelManager.cpp \
../Core/Src/CommandProcessor.cpp \
../Core/Src/Configuration.cpp \
../Core/Src/DataTerminal.cpp \
../Core/Src/EventQueue.cpp \
../Core/Src/Events.cpp \
../Core/Src/GPS.cpp \
../Core/Src/LEDManager.cpp \
../Core/Src/NMEAEncoder.cpp \
../Core/Src/NMEASentence.cpp \
../Core/Src/NoiseFloorDetector.cpp \
../Core/Src/RFIC.cpp \
../Core/Src/RXPacket.cpp \
../Core/Src/RXPacketProcessor.cpp \
../Core/Src/RadioManager.cpp \
../Core/Src/Receiver.cpp \
../Core/Src/TXPacket.cpp \
../Core/Src/TXScheduler.cpp \
../Core/Src/Transceiver.cpp \
../Core/Src/Utils.cpp \
../Core/Src/arbitrary_tx.cpp \
../Core/Src/main.cpp \
../Core/Src/printf_serial.cpp \
../Core/Src/si4460.cpp \
../Core/Src/si4463.cpp \
../Core/Src/si4467.cpp 

C_SRCS += \
../Core/Src/stm32l4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32l4xx.c 

C_DEPS += \
./Core/Src/stm32l4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32l4xx.d 

OBJS += \
./Core/Src/AISMessages.o \
./Core/Src/AODV_mesh.o \
./Core/Src/ChannelManager.o \
./Core/Src/CommandProcessor.o \
./Core/Src/Configuration.o \
./Core/Src/DataTerminal.o \
./Core/Src/EventQueue.o \
./Core/Src/Events.o \
./Core/Src/GPS.o \
./Core/Src/LEDManager.o \
./Core/Src/NMEAEncoder.o \
./Core/Src/NMEASentence.o \
./Core/Src/NoiseFloorDetector.o \
./Core/Src/RFIC.o \
./Core/Src/RXPacket.o \
./Core/Src/RXPacketProcessor.o \
./Core/Src/RadioManager.o \
./Core/Src/Receiver.o \
./Core/Src/TXPacket.o \
./Core/Src/TXScheduler.o \
./Core/Src/Transceiver.o \
./Core/Src/Utils.o \
./Core/Src/arbitrary_tx.o \
./Core/Src/main.o \
./Core/Src/printf_serial.o \
./Core/Src/si4460.o \
./Core/Src/si4463.o \
./Core/Src/si4467.o \
./Core/Src/stm32l4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32l4xx.o 

CPP_DEPS += \
./Core/Src/AISMessages.d \
./Core/Src/AODV_mesh.d \
./Core/Src/ChannelManager.d \
./Core/Src/CommandProcessor.d \
./Core/Src/Configuration.d \
./Core/Src/DataTerminal.d \
./Core/Src/EventQueue.d \
./Core/Src/Events.d \
./Core/Src/GPS.d \
./Core/Src/LEDManager.d \
./Core/Src/NMEAEncoder.d \
./Core/Src/NMEASentence.d \
./Core/Src/NoiseFloorDetector.d \
./Core/Src/RFIC.d \
./Core/Src/RXPacket.d \
./Core/Src/RXPacketProcessor.d \
./Core/Src/RadioManager.d \
./Core/Src/Receiver.d \
./Core/Src/TXPacket.d \
./Core/Src/TXScheduler.d \
./Core/Src/Transceiver.d \
./Core/Src/Utils.d \
./Core/Src/arbitrary_tx.d \
./Core/Src/main.d \
./Core/Src/printf_serial.d \
./Core/Src/si4460.d \
./Core/Src/si4463.d \
./Core/Src/si4467.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.cpp Core/Src/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L432xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O3 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L432xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/AISMessages.cyclo ./Core/Src/AISMessages.d ./Core/Src/AISMessages.o ./Core/Src/AISMessages.su ./Core/Src/AODV_mesh.cyclo ./Core/Src/AODV_mesh.d ./Core/Src/AODV_mesh.o ./Core/Src/AODV_mesh.su ./Core/Src/ChannelManager.cyclo ./Core/Src/ChannelManager.d ./Core/Src/ChannelManager.o ./Core/Src/ChannelManager.su ./Core/Src/CommandProcessor.cyclo ./Core/Src/CommandProcessor.d ./Core/Src/CommandProcessor.o ./Core/Src/CommandProcessor.su ./Core/Src/Configuration.cyclo ./Core/Src/Configuration.d ./Core/Src/Configuration.o ./Core/Src/Configuration.su ./Core/Src/DataTerminal.cyclo ./Core/Src/DataTerminal.d ./Core/Src/DataTerminal.o ./Core/Src/DataTerminal.su ./Core/Src/EventQueue.cyclo ./Core/Src/EventQueue.d ./Core/Src/EventQueue.o ./Core/Src/EventQueue.su ./Core/Src/Events.cyclo ./Core/Src/Events.d ./Core/Src/Events.o ./Core/Src/Events.su ./Core/Src/GPS.cyclo ./Core/Src/GPS.d ./Core/Src/GPS.o ./Core/Src/GPS.su ./Core/Src/LEDManager.cyclo ./Core/Src/LEDManager.d ./Core/Src/LEDManager.o ./Core/Src/LEDManager.su ./Core/Src/NMEAEncoder.cyclo ./Core/Src/NMEAEncoder.d ./Core/Src/NMEAEncoder.o ./Core/Src/NMEAEncoder.su ./Core/Src/NMEASentence.cyclo ./Core/Src/NMEASentence.d ./Core/Src/NMEASentence.o ./Core/Src/NMEASentence.su ./Core/Src/NoiseFloorDetector.cyclo ./Core/Src/NoiseFloorDetector.d ./Core/Src/NoiseFloorDetector.o ./Core/Src/NoiseFloorDetector.su ./Core/Src/RFIC.cyclo ./Core/Src/RFIC.d ./Core/Src/RFIC.o ./Core/Src/RFIC.su ./Core/Src/RXPacket.cyclo ./Core/Src/RXPacket.d ./Core/Src/RXPacket.o ./Core/Src/RXPacket.su ./Core/Src/RXPacketProcessor.cyclo ./Core/Src/RXPacketProcessor.d ./Core/Src/RXPacketProcessor.o ./Core/Src/RXPacketProcessor.su ./Core/Src/RadioManager.cyclo ./Core/Src/RadioManager.d ./Core/Src/RadioManager.o ./Core/Src/RadioManager.su ./Core/Src/Receiver.cyclo ./Core/Src/Receiver.d ./Core/Src/Receiver.o ./Core/Src/Receiver.su ./Core/Src/TXPacket.cyclo ./Core/Src/TXPacket.d ./Core/Src/TXPacket.o ./Core/Src/TXPacket.su ./Core/Src/TXScheduler.cyclo ./Core/Src/TXScheduler.d ./Core/Src/TXScheduler.o ./Core/Src/TXScheduler.su ./Core/Src/Transceiver.cyclo ./Core/Src/Transceiver.d ./Core/Src/Transceiver.o ./Core/Src/Transceiver.su ./Core/Src/Utils.cyclo ./Core/Src/Utils.d ./Core/Src/Utils.o ./Core/Src/Utils.su ./Core/Src/arbitrary_tx.cyclo ./Core/Src/arbitrary_tx.d ./Core/Src/arbitrary_tx.o ./Core/Src/arbitrary_tx.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/printf_serial.cyclo ./Core/Src/printf_serial.d ./Core/Src/printf_serial.o ./Core/Src/printf_serial.su ./Core/Src/si4460.cyclo ./Core/Src/si4460.d ./Core/Src/si4460.o ./Core/Src/si4460.su ./Core/Src/si4463.cyclo ./Core/Src/si4463.d ./Core/Src/si4463.o ./Core/Src/si4463.su ./Core/Src/si4467.cyclo ./Core/Src/si4467.d ./Core/Src/si4467.o ./Core/Src/si4467.su ./Core/Src/stm32l4xx_it.cyclo ./Core/Src/stm32l4xx_it.d ./Core/Src/stm32l4xx_it.o ./Core/Src/stm32l4xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32l4xx.cyclo ./Core/Src/system_stm32l4xx.d ./Core/Src/system_stm32l4xx.o ./Core/Src/system_stm32l4xx.su

.PHONY: clean-Core-2f-Src

