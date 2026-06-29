################################
# LightWeight Graphics Library #
################################
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS src/*.cpp src/*.h)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" PREFIX "Source Files" FILES ${SRC_FILES})
file(GLOB_RECURSE INC_FILES CONFIGURE_DEPENDS include/LWGL/*.h)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/include/LWGL" PREFIX "Include" FILES ${INC_FILES})

add_library(lwgl STATIC ${SRC_FILES})
target_sources(lwgl PUBLIC ${INC_FILES})

# Exhaustive switch on enums: new enumerators must get an explicit case (even if a default exists).
# GCC/Clang: -Wswitch-enum; MSVC: C4061/C4062 (enable as errors).
target_compile_options(
    lwgl
    PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/we4061>
        $<$<CXX_COMPILER_ID:MSVC>:/we4062>
        $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:-Werror=switch>
        $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:-Werror=switch-enum>
)

target_link_libraries(lwgl PRIVATE glfw)
target_link_libraries(lwgl PRIVATE glad)
target_link_libraries(lwgl PRIVATE glm)

target_include_directories(
    lwgl SYSTEM PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_include_directories(lwgl SYSTEM PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/libs>)
# target_include_directories(lwgl SYSTEM PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/libs)
