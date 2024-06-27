# Open CV Configuration Guide for CMake

Ensure that the relevant resources are installed using:

`sudo apt update`

`sudo apt install cmake g++ libopencv-dev`

### Follow these steps
- Make a Directory and create a project
- Create a code file (e.g. main.cpp)
- Create a CMakeLists.txt file
   - Ensure the add_executables instruction has the correct path
- `Ctrl` + `Shift` + `P` to open Command Palette and do `CMake: Configure`
   - Let VSCode find the compiler
   - Select your CMakeLists.txt file in the correct directory
- Create c_cpp_properties.json
   - Ensure the directory has the correct path to the OpenCV header files
- Press `F7` to build the code
- Press `Shift` + `F5` to run the code without Debugging
 
- To be able to run with Debugging, create a `launch.json` file


# Useful Links
[Install CMake on Linux](https://docs.opencv.org/4.x/db/df5/tutorial_linux_gcc_cmake.html)

[Getting Started with CMake](https://cmake.org/getting-started/)
