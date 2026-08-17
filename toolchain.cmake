set(CMAKE_SYSTEM_NAME       Generic)
set(CMAKE_SYSTEM_PROCESSOR  arm)
set(CMAKE_SYSTEM_VERSION    1)

set(TOOLCHAIN_PREFIX "arm-none-eabi-" CACHE STRING "Cross compiler prefix")
set(TOOLCHAIN_PATH   ""               CACHE PATH   "Dir holding the cross compiler; empty = search PATH")

if (TOOLCHAIN_PATH)
    set(_tc_hints HINTS "${TOOLCHAIN_PATH}" NO_DEFAULT_PATH)
endif ()

find_program(CMAKE_C_COMPILER   ${TOOLCHAIN_PREFIX}gcc     ${_tc_hints} REQUIRED)
find_program(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}g++     ${_tc_hints} REQUIRED)
find_program(CMAKE_ASM_COMPILER ${TOOLCHAIN_PREFIX}gcc     ${_tc_hints} REQUIRED)
find_program(CMAKE_AR           ${TOOLCHAIN_PREFIX}ar      ${_tc_hints} REQUIRED)
find_program(CMAKE_RANLIB       ${TOOLCHAIN_PREFIX}ranlib  ${_tc_hints} REQUIRED)
find_program(CMAKE_OBJCOPY      ${TOOLCHAIN_PREFIX}objcopy ${_tc_hints})
find_program(CMAKE_OBJDUMP      ${TOOLCHAIN_PREFIX}objdump ${_tc_hints})
find_program(CMAKE_SIZE         ${TOOLCHAIN_PREFIX}size    ${_tc_hints})

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

set(CMAKE_EXECUTABLE_SUFFIX_C   ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_CXX ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_ASM ".elf")

set(STM32_ARCH_FLAGS   "-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard")
set(STM32_COMMON_FLAGS "${STM32_ARCH_FLAGS} -Wall -ffunction-sections -fdata-sections")

set(CMAKE_C_FLAGS_INIT   "${STM32_COMMON_FLAGS}")
set(CMAKE_CXX_FLAGS_INIT "${STM32_COMMON_FLAGS} -fno-rtti -fno-exceptions -fno-threadsafe-statics -fno-use-cxa-atexit")
set(CMAKE_ASM_FLAGS_INIT "${STM32_ARCH_FLAGS} -x assembler-with-cpp")

set(CMAKE_C_FLAGS_DEBUG_INIT     "-Og -g3")
set(CMAKE_C_FLAGS_RELEASE_INIT   "-Os -g0 -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG_INIT   "-Og -g3")
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-Os -g0 -DNDEBUG")

set(CMAKE_EXE_LINKER_FLAGS_INIT "${STM32_ARCH_FLAGS} --specs=nano.specs -Wl,--gc-sections")
