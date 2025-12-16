cmake_minimum_required(VERSION 3.20)
project(Grove VERSION 1.0.0)

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

add_executable(Grove)
target_sources(Grove PRIVATE ${MY_SOURCES} "src/Renderer/Shader.h" "src/Renderer/Shader.cpp" "src/Renderer/VBO.h" "src/Renderer/VBO.cpp" "src/Renderer/EBO.h" "src/Renderer/EBO.cpp" "src/Renderer/VAO.h" "src/Renderer/VAO.cpp" "src/Renderer/Camera.h" "src/Core/Window.h" "src/Core/Window.cpp" "src/Core/Application.h" "src/Core/Application.cpp" "src/World/Voxel.h" "src/World/Chunk.h" "src/World/Chunk.cpp" "src/World/ChunkManager.h" "src/World/ChunkManager.cpp")

target_include_directories(Grove PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/include/"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/GLFW/include"
)

target_link_libraries(Grove PRIVATE 
    glm 
    glfw
    glad 
    stb_image 
)

set(TARGET_NAME Grove) 

add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/resources
    $<TARGET_FILE_DIR:${TARGET_NAME}>/resources
    COMMENT "Copying resources to output directory..."
)

# Compiler Specific Fixes
if(MSVC)
    target_compile_definitions(Grove PUBLIC _CRT_SECURE_NO_WARNINGS)
endif()