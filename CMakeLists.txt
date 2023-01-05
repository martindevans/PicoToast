cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
include(pico_sdk_import.cmake)
include(pico_extras_import.cmake)

project(toast)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(sprite)
add_subdirectory(levels)
add_subdirectory(content)
add_subdirectory(physics)
add_subdirectory(audio)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

add_executable(toast
    sprite_demo.c
)

target_include_directories(toast PRIVATE ${CMAKE_CURRENT_LIST_DIR})

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(toast
    pico_stdlib
    pico_audio_i2s
    pico_scanvideo_dpi
    pico_multicore
    sprite
    tinyusb_host
    tinyusb_board
    audio
)

target_compile_definitions(toast PRIVATE
    # Disable Pico alarm functions to save some binary space (https://raspberrypi.github.io/pico-sdk-doxygen/group__alarm.html)
    PICO_TIME_DEFAULT_ALARM_POOL_DISABLED=0

    # Disable checking if this is running on aimsulated Pico hardware to save some binary space
    PICO_NO_FPGA_CHECK=1

    # Set the size of the scanline buffer
    PICO_SCANVIDEO_MAX_SCANLINE_BUFFER_WORDS=500

    # Use single core rendering
    SCANLINE_RENDER_MONO

    # How many DMAs to claim for rendering (max)
    SCANLINE_RENDER_PARALLEL_DMA=10

    # Display debug test on screen
    DEBUG
)

# Send Serial output nowhere
pico_enable_stdio_usb(toast 0)
pico_enable_stdio_uart(toast 0)

pico_add_extra_outputs(toast)

