cmake_minimum_required(VERSION 3.20)
project(VoxelEngine VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(vendor/GLFW)
add_subdirectory(vendor/glad)
add_subdirectory(vendor/stb_image)
add_subdirectory(vendor/glm)

file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable(VoxelEngine)
target_sources(VoxelEngine PRIVATE ${MY_SOURCES})

target_include_directories(VoxelEngine PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/include/"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/GLFW/include"
)

target_link_libraries(VoxelEngine PRIVATE 
    glm 
    glfw
    glad 
    stb_image 
)

# Compiler Specific Fixes
if(MSVC)
    target_compile_definitions(VoxelEngine PUBLIC _CRT_SECURE_NO_WARNINGS)
endif()

target_compile_definitions(VoxelEngine PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")