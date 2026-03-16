cmake_minimum_required(VERSION 4.1)
project(RayTracing)

set(CMAKE_CXX_STANDARD 20)

# Define SOURCES to be a list of all the source files for the project
file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
add_executable(${PROJECT_NAME} "${MY_SOURCES}")

# Include files
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")

