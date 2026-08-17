set(STM32F4_HALPP_TEST_LD ${PROJECT_SOURCE_DIR}/STM32F446RETX_FLASH.ld
        CACHE FILEPATH "Linker script for test firmware")

function(add_test_firmware name)
    cmake_parse_arguments(PARSE_ARGV 1 ARG "" "LINKER_SCRIPT" "SOURCES")

    if (NOT ARG_SOURCES)
        message(FATAL_ERROR "add_test_firmware(${name}): SOURCES is required")
    endif ()
    if (NOT ARG_LINKER_SCRIPT)
        set(ARG_LINKER_SCRIPT ${STM32F4_HALPP_TEST_LD})
    endif ()

    add_executable(${name} ${ARG_SOURCES})

    # startup brings the vector table, system_stm32f4xx.c and syscalls.c in as
    # INTERFACE sources; cmsis headers + STM32F446xx arrive transitively.
    target_link_libraries(${name} PRIVATE
            stm32f4_halpp::stm32f4_halpp
            stm32f4_halpp::startup)

    target_link_options(${name} PRIVATE
            -T${ARG_LINKER_SCRIPT}
            -Wl,-Map=${name}.map,--cref
            -Wl,--print-memory-usage)

    # Relink when the script changes — otherwise you debug a stale image.
    set_target_properties(${name} PROPERTIES
            LINK_DEPENDS ${ARG_LINKER_SCRIPT}
            SUFFIX       ".elf")

    # .hex for st-flash / .bin for dfu-util, plus a size summary.
    add_custom_command(TARGET ${name} POST_BUILD
            COMMAND ${CMAKE_OBJCOPY} -O ihex   $<TARGET_FILE:${name}> $<TARGET_FILE_BASE_NAME:${name}>.hex
            COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${name}> $<TARGET_FILE_BASE_NAME:${name}>.bin
            COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${name}>
            VERBATIM)
endfunction()

add_test_firmware(gpio_test SOURCES gpio_test/gpio_test_ld2.cpp)
add_test_firmware(uart_test SOURCES uart_test/uart_tx_test.cpp)
add_test_firmware(spi_test  SOURCES spi_test/spi_test1.cpp)

# A test that needs a different memory layout just says so:
# add_test_firmware(ram_test
#         SOURCES       ram_test/ram_test.cpp
#         LINKER_SCRIPT ${PROJECT_SOURCE_DIR}/STM32F446RETX_RAM.ld)

add_custom_target(tests DEPENDS gpio_test uart_test spi_test)