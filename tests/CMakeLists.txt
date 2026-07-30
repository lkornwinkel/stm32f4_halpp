


#set(STARTUP_FILE ${CMAKE_SOURCE_DIR}/tests/startup_stm32f446xx.s)
#set(SYSTEM_FILE ${CMAKE_SOURCE_DIR}/tests/system_stm32f4xx.c)


add_executable(gpio_test
        gpio_test/gpio_test_ld2.cpp
        ${STARTUP_FILE}
        ${SYSTEM_FILE}
        ${SYSCALL_FILE}
)

target_include_directories(gpio_test
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
        PUBLIC ${CMSIS_SOURCES_PATH}/Include
        PUBLIC ${CORE_SOURCES_PATH}/Core/Include
        PUBLIC ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(gpio_test PRIVATE cmsis stm32f4_hal)

set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/STM32F446RETX_FLASH.ld)
set_target_properties(gpio_test PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT})
target_link_options(gpio_test PRIVATE -T${LINKER_SCRIPT} -Wl,-Map=gpio_test.map,--cref -Wl,--gc-sections)


add_executable(uart_test
        uart_test/uart_tx_test.cpp
        ${STARTUP_FILE}
        ${SYSTEM_FILE}
        ${SYSCALL_FILE}
)

target_include_directories(uart_test
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
        PUBLIC ${CMSIS_SOURCES_PATH}/Include
        PUBLIC ${CORE_SOURCES_PATH}/Core/Include
        PUBLIC ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(uart_test PRIVATE cmsis stm32f4_hal)

set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/STM32F446RETX_FLASH.ld)
set_target_properties(uart_test PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT})
target_link_options(uart_test PRIVATE -T${LINKER_SCRIPT} -Wl,-Map=gpio_test.map,--cref -Wl,--gc-sections)


add_executable(spi_test
        spi_test/spi_test1.cpp
        ${STARTUP_FILE}
        ${SYSTEM_FILE}
        ${SYSCALL_FILE}
)

target_include_directories(spi_test
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
        PUBLIC ${CMSIS_SOURCES_PATH}/Include
        PUBLIC ${CORE_SOURCES_PATH}/Core/Include
        PUBLIC ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(spi_test PRIVATE cmsis stm32f4_hal)

set(LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/STM32F446RETX_FLASH.ld")
set_target_properties(spi_test PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT})
target_link_options(spi_test PRIVATE -T${LINKER_SCRIPT} -Wl,-Map=gpio_test.map,--cref -Wl,--gc-sections)



add_executable(dpy_black_test
        ${STARTUP_FILE}
        ${SYSTEM_FILE}
        ${SYSCALL_FILE}
)

target_include_directories(dpy_black_test
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
        PUBLIC ${CMSIS_SOURCES_PATH}/Include
        PUBLIC ${CORE_SOURCES_PATH}/Core/Include
        PUBLIC ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(dpy_black_test PRIVATE cmsis stm32f4_hal)

set(LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/STM32F446RETX_FLASH.ld")
set_target_properties(dpy_black_test PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT})
target_link_options(dpy_black_test PRIVATE -T${LINKER_SCRIPT} -Wl,-Map=gpio_test.map,--cref -Wl,--gc-sections)
