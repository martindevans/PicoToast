add_library(content INTERFACE)

target_sources(content INTERFACE
        melon.h
)

target_include_directories(content INTERFACE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(content INTERFACE pico_base_headers hardware_interp)
