# IoT-Air-Quality-Monitoring
An Internet of Things (IoT) air quality monitoring project with STM32F103C6T6A (blue pill dev board) that measures the temperature, humidity, VOC, and eCO2 of a room and sends the data to a Postgresql database via HTTP post request. 



## Hardware Overview
- MCU: STM32F103C6T6A
- Air Quality Sensor: SGP30 (I2C)
- Temperature and Humidty sensor: AM2320 (I2C)
- Wireless Module: ESP-01 (UART)

## Development Tools/Workflow
- OS: Linux (Arch)
- IDE: VSCode
    - Extensions:
        - C/C++ Intellisense
        - Cortex-Debug
        - Makefile
- Other Tools:
    - OpenOCD
    - Arm GNU Toolchain (gcc-arm-none-eabi, etc.)
    - CubeMX (generating code and libraries)
    - Make
