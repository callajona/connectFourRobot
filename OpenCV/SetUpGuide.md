# Open CV Configuration Guide for CMake


## CMakeLists.txt
`cmake_minimum_required(VERSION 3.10)

project(MyOpenCVProject)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find OpenCV package
find_package(OpenCV REQUIRED)

# Include Directririons
include_directories( ${OpenCV_INCLUDE_DIRS} )

# Add source files --------------------------------- Change file name and or path
add_executable(${PROJECT_NAME} main.cpp)

# Link OpenCV libraries
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
`
