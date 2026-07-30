
set(TOOLCHAIN_PREFIX arm-none-eabi-)

set(TOOLCHAIN_PATH "")
set(CMAKE_C_COMPILER    ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_CXX_COMPILER  ${TOOLCHAIN_PREFIX}g++)
set(CMAKE_ASM_COMPILER  ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_AR            ${TOOLCHAIN_PREFIX}ar)
set(CMAKE_OBJCOPY       ${TOOLCHAIN_PREFIX}objcopy)
set(CMAKE_OBJDUMP       ${TOOLCHAIN_PREFIX}objdump)
set(SIZE                ${TOOLCHAIN_PREFIX}size)

set(CMAKE_EXECUTABLE_SUFFIX_ASM     ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_C       ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_CXX     ".elf")

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(TARGET_FLAGS "-mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard ")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${TARGET_FLAGS}")
set(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS} -x assembler-with-cpp -MMD -MP")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -fdata-sections -ffunction-sections")

set(CMAKE_C_FLAGS_DEBUG "-O0 -g3")
set(CMAKE_C_FLAGS_RELEASE "-Os -g0")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g3")
set(CMAKE_CXX_FLAGS_RELEASE "-Os -g0")

set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -fno-rtti -fno-exceptions -fno-threadsafe-statics")

set(CMAKE_EXE_LINKER_FLAGS "${TARGET_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T \"${CMAKE_SOURCE_DIR}/STM32F446XX_FLASH.ld\"")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --specs=nano.specs")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Map=${CMAKE_PROJECT_NAME}.map -Wl,--gc-sections")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--print-memory-usage")
set(TOOLCHAIN_LINK_LIBRARIES "m")

add_definitions("-DSTM32F446xx")
#set(MCPU cortex-m4)
#
#set(COMMON_FLAGS "-mcpu=${MCPU} -mthumb -mthumb-interwork -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -g -fno-rtti -fno-exceptions -ffunction-sections -fdata-sections -fno-common -fmessage-length=0 -DCORE_CM4 -DDEBUG -DSTM32F446xx -DHSE_VALUE=8000000 -DARM_MATH_CM4 -DARM_MATH_MATRIX_CHECK -DARM_MATH_ROUNDING")
#
#set(CMAKE_C_FLAGS_INIT "${COMMON_FLAGS}")
#set(CMAKE_CXX_FLAGS_INIT "${COMMON_FLAGS}")
#set(CMAKE_ASM_FLAGS_INIT "-mcpu=${MCPU} -mthumb -mthumb-interwork -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -g")
#
#set(CMAKE_EXE_LINKER_FLAGS_INIT "-mcpu=${MCPU} -mthumb -mthumb-interwork -mfloat-abi=hard -mfpu=fpv4-sp-d16 --specs=nosys.specs")
#
#add_definitions(-DCORE_CM4 -DDEBUG -DSTM32F446xx)


