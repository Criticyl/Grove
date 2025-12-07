cmake_minimum_required(VERSION 3.10)
project(stb_image)

# Create the library from the source file
add_library(stb_image src/stb_image.cpp)

# Point the compiler to the 'include' folder where the header lives
target_include_directories(stb_image PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)