add_library(sprite INTERFACE)

target_sources(sprite INTERFACE
        $<$<BOOL:${PICO_ON_DEVICE}>:${CMAKE_CURRENT_LIST_DIR}/sprite.S>
        ${CMAKE_CURRENT_LIST_DIR}/sprite.c
        ${CMAKE_CURRENT_LIST_DIR}/sprite.h
        ${CMAKE_CURRENT_LIST_DIR}/sprite_dma.c
        ${CMAKE_CURRENT_LIST_DIR}/sprite_dma.h
        ${CMAKE_CURRENT_LIST_DIR}/scanline_rendering.c
        ${CMAKE_CURRENT_LIST_DIR}/scanline_rendering.h
)

target_include_directories(sprite INTERFACE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(sprite INTERFACE pico_base_headers hardware_interp)
