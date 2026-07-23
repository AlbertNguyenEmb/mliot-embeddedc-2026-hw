cmake_minimum_required(VERSION 3.22)

####################################################
# Project
####################################################

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

####################################################
# Toolchain
####################################################

include(${CMAKE_SOURCE_DIR}/cmake/gcc-arm-none-eabi.cmake)

project(gpio_blink C ASM)
####################################################
# Executable
####################################################

add_executable(${PROJECT_NAME})

####################################################
# MCU Defines
####################################################

target_compile_definitions(${PROJECT_NAME}
PRIVATE
    STM32F103xB
    USE_HAL_DRIVER
)

####################################################
# Include Paths
####################################################

target_include_directories(${PROJECT_NAME}
PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/STM32CubeF1/Drivers/STM32F1xx_HAL_Driver/Inc
    ${CMAKE_SOURCE_DIR}/STM32CubeF1/Drivers/CMSIS/Include
    ${CMAKE_SOURCE_DIR}/STM32CubeF1/Drivers/CMSIS/Device/ST/STM32F1xx/Include
)

####################################################
# Source Files
####################################################

target_sources(${PROJECT_NAME}
PRIVATE
    ${CMAKE_SOURCE_DIR}/main.c
    ${CMAKE_SOURCE_DIR}/system_stm32f1xx.c
    ${CMAKE_SOURCE_DIR}/startup_stm32f103xb.s

    ${CMAKE_SOURCE_DIR}/STM32CubeF1/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal.c
    ${CMAKE_SOURCE_DIR}/STM32CubeF1/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio.c
    ${CMAKE_SOURCE_DIR}/STM32CubeF1/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_cortex.c
    ${CMAKE_SOURCE_DIR}/STM32CubeF1/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc.c
)

####################################################
# Compiler Options
####################################################

target_compile_options(${PROJECT_NAME}
PRIVATE
    -Wall
    -Wextra
)

####################################################
# Generate HEX/BIN
####################################################

add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex
            $<TARGET_FILE:${PROJECT_NAME}>
            ${PROJECT_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary
            $<TARGET_FILE:${PROJECT_NAME}>
            ${PROJECT_NAME}.bin
    COMMENT "Generate HEX/BIN"
)

####################################################
# Print Size
####################################################

add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_SIZE}
            $<TARGET_FILE:${PROJECT_NAME}>
    COMMENT "Program Size"
)

add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY}
            -O binary
            $<TARGET_FILE:${CMAKE_PROJECT_NAME}>
            ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.bin
)