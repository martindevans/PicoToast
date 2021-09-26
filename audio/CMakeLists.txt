add_library(audio INTERFACE)

target_sources(audio INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/audio_renderer.h
)

target_include_directories(audio INTERFACE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(audio INTERFACE pico_base_headers hardware_interp)
