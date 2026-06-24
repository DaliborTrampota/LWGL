cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
    message(FATAL_ERROR "In-source builds are not allowed. Please create a separate build directory.")
endif()

project(LWGLTests VERSION 1.0)

# Set runtime output directory
# set(PROJECT_RUNTIME_OUTPUT_DIR "${CMAKE_BINARY_DIR}/Stomata/$<CONFIG>")

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp src/*.h)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" PREFIX "Source Files" FILES ${SOURCES})

# Game resources
# file(GLOB_RECURSE SHADER_FILES CONFIGURE_DEPENDS shaders/*.glsl)
# file(GLOB_RECURSE RESOURCE_FILES CONFIGURE_DEPENDS resources/*)

# source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/shaders" PREFIX "Shaders" FILES ${SHADER_FILES})
# source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/resources" PREFIX "Resources" FILES ${RESOURCE_FILES})

add_executable(LWGLTests WIN32 ${SOURCES})
target_link_options(LWGLTests PRIVATE "/SUBSYSTEM:CONSOLE")

target_link_libraries(LWGLTests PRIVATE lwgl)

# target_include_directories(LWGLTests SYSTEM PUBLIC "${CMAKE_SOURCE_DIR}/voxel_engine/engine/src")

# target_sources(Stomata PRIVATE ${RESOURCE_FILES})
# target_sources(Stomata PRIVATE ${SHADER_FILES})

# setup_voxelengine_resource_copy(Stomata)
# setup_seuil_resource_copy(Stomata)

# add_custom_command(TARGET Stomata POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_directory
#         ${CMAKE_CURRENT_SOURCE_DIR}/resources $<TARGET_FILE_DIR:Stomata>/resources
# )

# Set debugger working directory
set_target_properties(LWGLTests PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:LWGLTests>"
)

# Register test with CTest
add_test(NAME LWGLTests COMMAND LWGLTests)
