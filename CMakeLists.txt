cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)
#add_subdirectory(i2c_slave)
project(m6502monitor C CXX ASM)
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(${PROJECT_NAME})
#configure_file(memory_config.h.in memory_config.h)

target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall
        -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
        -Wno-unused-function # we have some for the docs that aren't called
        -Wno-maybe-uninitialized
)

target_include_directories(${PROJECT_NAME} PUBLIC
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_BINARY_DIR}
)

target_sources(${PROJECT_NAME} PRIVATE
        src/main.cpp
        src/io.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        pico_stdlib
        hardware_pio
        hardware_gpio
)

pico_generate_pio_header(${PROJECT_NAME} ${CMAKE_CURRENT_LIST_DIR}/pio/io.pio)

pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_add_extra_outputs(${PROJECT_NAME})
