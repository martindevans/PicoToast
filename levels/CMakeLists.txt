add_library(levels INTERFACE)

target_sources(levels INTERFACE
        level1.h
        level2.h
)

target_include_directories(levels INTERFACE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(levels INTERFACE pico_base_headers hardware_interp)
