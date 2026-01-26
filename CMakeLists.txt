################################
# LightWeight Graphics Library #
################################
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS src/*.cpp src/*.h)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" PREFIX "Source Files" FILES ${SRC_FILES})
file(GLOB_RECURSE INC_FILES CONFIGURE_DEPENDS include/LWGL/*.h)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/include/LWGL" PREFIX "Include" FILES ${INC_FILES})

add_library(lwgl STATIC ${SRC_FILES})
target_sources(lwgl PUBLIC ${INC_FILES})

target_link_libraries(lwgl PUBLIC glfw)
target_link_libraries(lwgl PUBLIC glad)
target_link_libraries(lwgl PUBLIC glm)


target_include_directories(
    lwgl SYSTEM
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/dependencies
)


# Optional testing
if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()