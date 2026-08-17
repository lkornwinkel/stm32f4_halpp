cmake_minimum_required(VERSION 3.28)

project(stm32f4_halpp
        VERSION 0.1.0
        DESCRIPTION "C++ HAL for STM32F4"
        LANGUAGES C CXX ASM)

#option(STM32F4_HALPP_BUILD_TESTS "Build on-target test firmware" ${PROJECT_IS_TOP_LEVEL})
#option(STM32F4_HALPP_INSTALL     "Generate install/export rules" ${PROJECT_IS_TOP_LEVEL})
set(STM32F4_HALPP_DEVICE "STM32F446xx" CACHE STRING "CMSIS device macro")

if (PROJECT_IS_TOP_LEVEL)
    if (NOT CMAKE_CROSSCOMPILING)
        message(FATAL_ERROR
                "Configure with a toolchain, e.g. --preset debug or "
                "-DCMAKE_TOOLCHAIN_FILE=${CMAKE_CURRENT_SOURCE_DIR}/toolchain.cmake")
    endif ()
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif ()

set(CMSIS_DEVICE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/extern/cmsis)
set(CMSIS_CORE_DIR   ${CMAKE_CURRENT_SOURCE_DIR}/extern/core)

if (NOT EXISTS ${CMSIS_DEVICE_DIR}/Include)
    message(FATAL_ERROR "extern/cmsis is empty — run: git submodule update --init --recursive")
endif ()

add_library(stm32f4_halpp_cmsis INTERFACE)
add_library(stm32f4_halpp::cmsis ALIAS stm32f4_halpp_cmsis)

target_include_directories(stm32f4_halpp_cmsis SYSTEM INTERFACE
        $<BUILD_INTERFACE:${CMSIS_DEVICE_DIR}/Include>
        $<BUILD_INTERFACE:${CMSIS_CORE_DIR}/CMSIS/Core/Include>
        $<INSTALL_INTERFACE:include>)
target_compile_definitions(stm32f4_halpp_cmsis INTERFACE ${STM32F4_HALPP_DEVICE})


add_library(stm32f4_halpp_startup INTERFACE)
add_library(stm32f4_halpp::startup ALIAS stm32f4_halpp_startup)

target_sources(stm32f4_halpp_startup INTERFACE
        $<BUILD_INTERFACE:${CMSIS_DEVICE_DIR}/Source/Templates/gcc/startup_stm32f446xx.s>
        $<BUILD_INTERFACE:${CMSIS_DEVICE_DIR}/Source/Templates/system_stm32f4xx.c>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/syscalls.c>)
target_link_libraries(stm32f4_halpp_startup INTERFACE stm32f4_halpp::cmsis)

add_library(stm32f4_halpp_linker_script INTERFACE)
add_library(stm32f4_halpp::linker_script ALIAS stm32f4_halpp_linker_script)
set(STM32F4_HALPP_LD ${CMAKE_CURRENT_SOURCE_DIR}/STM32F446XX_FLASH.ld)
target_link_options(stm32f4_halpp_linker_script INTERFACE
        -T${STM32F4_HALPP_LD} --specs=nano.specs -Wl,--gc-sections -Wl,--print-memory-usage)
set_target_properties(stm32f4_halpp_linker_script PROPERTIES
        INTERFACE_LINK_DEPENDS ${STM32F4_HALPP_LD})


add_library(stm32f4_halpp STATIC
        src/gpio.cpp
        src/clock.cpp
        src/uart.cpp
        src/spi.cpp)
add_library(stm32f4_halpp::stm32f4_halpp ALIAS stm32f4_halpp)

target_sources(stm32f4_halpp PUBLIC
        FILE_SET  hal_headers
        TYPE      HEADERS
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include
        FILES
        include/hal/gpio.hpp
        include/hal/clock.hpp
        include/hal/error.hpp
        include/hal/uart.hpp
        include/hal/spi.hpp
        include/hal/cmsis.hpp)

target_link_libraries(stm32f4_halpp PUBLIC stm32f4_halpp::cmsis)
target_compile_features(stm32f4_halpp PUBLIC cxx_std_26)
set_target_properties(stm32f4_halpp PROPERTIES
        CXX_SCAN_FOR_MODULES OFF
        EXPORT_NAME          stm32f4_halpp)

if (PROJECT_IS_TOP_LEVEL)
    set_target_properties(stm32f4_halpp PROPERTIES
            ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/output)
    file(CREATE_LINK ${CMAKE_BINARY_DIR}/compile_commands.json
            ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json SYMBOLIC)
endif ()


if (STM32F4_HALPP_BUILD_TESTS)
    add_subdirectory(tests)
endif ()

if (STM32F4_HALPP_INSTALL)
    include(GNUInstallDirs)
    include(CMakePackageConfigHelpers)

    install(TARGETS stm32f4_halpp stm32f4_halpp_cmsis
            EXPORT   stm32f4_halppTargets
            FILE_SET hal_headers)
    install(DIRECTORY ${CMSIS_DEVICE_DIR}/Include/ ${CMSIS_CORE_DIR}/CMSIS/Core/Include/
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
    install(EXPORT stm32f4_halppTargets
            NAMESPACE   stm32f4_halpp::
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/stm32f4_halpp)

    write_basic_package_version_file(stm32f4_halppConfigVersion.cmake
            COMPATIBILITY SameMajorVersion ARCH_INDEPENDENT)
    configure_package_config_file(cmake/stm32f4_halppConfig.cmake.in
            stm32f4_halppConfig.cmake
            INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/stm32f4_halpp)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/stm32f4_halppConfig.cmake
            ${CMAKE_CURRENT_BINARY_DIR}/stm32f4_halppConfigVersion.cmake
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/stm32f4_halpp)
endif ()


