file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS src/*.cpp src/*.h)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" PREFIX "Source Files" FILES ${SRC_FILES})
file(GLOB_RECURSE INC_FILES CONFIGURE_DEPENDS include/LWGL/*.h)
# source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/include/lwgl" PREFIX "Include" FILES ${INC_FILES})

add_library(lwgl STATIC ${SRC_FILES})
# target_sources(lwgl PRIVATE ${INC_FILES})

target_link_libraries(lwgl PRIVATE glfw)
target_link_libraries(lwgl PRIVATE glad)
target_link_libraries(lwgl PRIVATE glm)

target_include_directories(
    lwgl SYSTEM PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
# target_include_directories(lwgl 
#     PUBLIC
#         $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
#         $<INSTALL_INTERFACE:include>
#     PRIVATE
#         ${CMAKE_CURRENT_SOURCE_DIR}/src
#         ${CMAKE_CURRENT_SOURCE_DIR}/include/lwgl
#         ${CMAKE_CURRENT_SOURCE_DIR}/include/lwgl/texture
#         ${CMAKE_CURRENT_SOURCE_DIR}/include/lwgl/render
#         ${CMAKE_CURRENT_SOURCE_DIR}/include/lwgl/gl
#         ${CMAKE_CURRENT_SOURCE_DIR}/include/lwgl/events
# )
# target_include_directories(lwgl SYSTEM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)