# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(project.cmake)
include(pico_sdk_import.cmake)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.4.0")
  message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.4.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

project(Hack-Hour-Clock-Firmware C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Definitions for WiFi
add_definitions(-DWIFI_SSID=${WIFI_SSID})
add_definitions(-DWIFI_PASS=${WIFI_PASS})

# Add executable. Default name is the project name, version 0.1

add_executable(Hack-Hour-Clock-Firmware Hack-Hour-Clock-Firmware.c )

pico_set_program_name(Hack-Hour-Clock-Firmware "Hack-Hour-Clock-Firmware")
pico_set_program_version(Hack-Hour-Clock-Firmware "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(Hack-Hour-Clock-Firmware 0)
pico_enable_stdio_usb(Hack-Hour-Clock-Firmware 1)

# Add the standard library to the build
target_link_libraries(Hack-Hour-Clock-Firmware
        pico_stdlib)

# Add the standard include files to the build
target_include_directories(Hack-Hour-Clock-Firmware PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts or any other standard includes, if required
)

# Add any user requested libraries
target_link_libraries(Hack-Hour-Clock-Firmware 
        hardware_spi
        hardware_clocks
        pico_cyw43_arch_lwip_threadsafe_background
        )

pico_add_extra_outputs(Hack-Hour-Clock-Firmware)

