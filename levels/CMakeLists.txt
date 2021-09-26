add_library(levels INTERFACE)

target_include_directories(levels INTERFACE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(levels INTERFACE pico_base_headers hardware_interp)
