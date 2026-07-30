cmake_minimum_required(VERSION 3.28)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
project(stm32f4_hal C CXX ASM)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(TOOLCHAIN_PREFIX arm-none-eabi-)
set(TOOLCHAIN_PATH "/home/lars/opt/STM32CubeCLT/GNU-tools-for-STM32/bin")

include(toolchain.cmake)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(EXTERNAL_SOURCES_PATH ${CMAKE_CURRENT_SOURCE_DIR}/extern)
set(CMSIS_SOURCES_PATH ${EXTERNAL_SOURCES_PATH}/cmsis)
set(CORE_SOURCES_PATH ${EXTERNAL_SOURCES_PATH}/core)

set(STM32F4_HAL_SOURCES
        src/gpio.cpp
        src/clock.cpp
        src/uart.cpp
        src/spi.cpp
)

set(STM32F4_HAL_INCLUDES
        include/hal/gpio.hpp
        include/hal/clock.hpp
        include/hal/error.hpp
        include/hal/uart.hpp
        include/hal/spi.hpp
)
#
set(STARTUP_FILE ${CMSIS_SOURCES_PATH}/Source/Templates/gcc/startup_stm32f446xx.s)
set(SYSTEM_FILE ${CMSIS_SOURCES_PATH}/Source/Templates/system_stm32f4xx.c)
set(SYSCALL_FILE ${CMAKE_SOURCE_DIR}/syscalls.c)
set(CMSIS_SOURCES ${STARTUP_FILE} ${SYSTEM_FILE} ${SYSCALL_FILE})

add_library(cmsis STATIC ${CMSIS_SOURCES})

target_include_directories(cmsis
        PUBLIC ${CMSIS_SOURCES_PATH}/Include
        PUBLIC ${CORE_SOURCES_PATH}/Core/Include
)


set_target_properties(cmsis PROPERTIES LINKER_LANGUAGE CXX)

add_library(${PROJECT_NAME} STATIC ${STM32F4_HAL_SOURCES} ${STM32F4_HAL_INCLUDES})

target_include_directories(${PROJECT_NAME}
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
        PUBLIC ${CMSIS_SOURCES_PATH}/Include
        PUBLIC ${CORE_SOURCES_PATH}/Core/Include
        PUBLIC ${CMAKE_SOURCE_DIR}/include
)

add_subdirectory(tests)

