add_library(physics INTERFACE)

target_sources(levels INTERFACE
        aabb.h
)

target_include_directories(physics INTERFACE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(physics INTERFACE pico_base_headers hardware_interp)
