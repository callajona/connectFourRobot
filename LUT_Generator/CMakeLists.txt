cmake_minimum_required(VERSION 3.0)
project(c4Solver
        VERSION 0.0.1
        DESCRIPTION "The leading Hello World App"
        LANGUAGES CXX)  

#GLOBING - Scan directory and find files depending on criteria - e.g. ends in .cpp
# NOT RECCOMENDED!
#file(GLOB_RECURSE SOURCE_FILES src/*.cpp)
#add_executable(HelloAppBinary main.cpp ${SOURCE_FILES})

# Find OpenCV package
find_package(OpenCV REQUIRED)

# Include Directririons
include_directories( ${OpenCV_INCLUDE_DIRS})

#Target
add_executable(c4Solver main.cpp src/Solver.cpp) # Point CMake to executable soruce files

target_compile_features(c4Solver PRIVATE cxx_std_20) 
target_include_directories(c4Solver PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include) # Tell Cmake where header files are

# Link OpenCV libraries
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})