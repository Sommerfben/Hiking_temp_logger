# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NEVER EDIT THE NEXT LINES for Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.0.0)
set(toolchainVersion 13_2_Rel1)
set(picotoolVersion 2.0.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(Hiking_temp_logger C CXX ASM)

add_subdirectory(${CMAKE_SOURCE_DIR}/lib/lvgl-release-v9.2)
# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(Hiking_temp_logger Hiking_temp_logger.cpp lib/temperature/temp_shtc3.cpp lib/LCD/GUI_Paint.cpp lib/LCD/LCD_Driver.cpp lib/LCD/DEV_Config.cpp)

pico_set_program_name(Hiking_temp_logger "Hiking_temp_logger")
pico_set_program_version(Hiking_temp_logger "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(Hiking_temp_logger 1)
pico_enable_stdio_usb(Hiking_temp_logger 0)


# Add the standard library to the build
target_link_libraries(Hiking_temp_logger
        pico_stdlib
        lvgl)
        

# Add the standard include files to the build
target_include_directories(Hiking_temp_logger PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts or any other standard includes, if required
  ${CMAKE_CURRENT_LIST_DIR}/lib/lvgl-release-v9.2 # for our common lwipopts or any other standard includes, if required
)

include_directories(${PROJECT_SOURCE_DIR}/lib/lvgl-release-v9.2)

# Add any user requested libraries
target_link_libraries(Hiking_temp_logger 
        hardware_spi
        hardware_i2c
        hardware_dma
        hardware_timer
        hardware_clocks
        pico_cyw43_arch_none
        lvgl
        )

target_sources(Hiking_temp_logger PRIVATE ${CMAKE_SOURCE_DIR}/lib/lvgl-release-v9.2/examples/styles/lv_example_style_8.c ${CMAKE_SOURCE_DIR}/lib/lvgl-release-v9.2/examples/get_started/lv_example_get_started_1.c)

pico_add_extra_outputs(Hiking_temp_logger)

