# Axis Render Engine

Axis Render is a rasterization-based 3D model rendering engine designed to visualize OBJ models. This project serves as an initial step in developing a game engine, providing the foundational tools for rendering and interacting with 3D models. As a standalone application, Axis Render offers a straightforward UI that allows users to manipulate and explore 3D models in real-time. With features like free-roam camera movement, real-time shader application, dynamic lighting adjustments, and model transformations, it provides a flexible platform for visualizing and fine-tuning 3D assets, laying the groundwork for more advanced game engine development.

## Navigation
- [Demo](#demo)
- [Feature Demo](#Feature Demo)
- [Installation](#installation)
  - [MacOS](#macos)
  - [Windows](#windows)
    - [MinGW](#for-mingw)
    - [Visual Studio](#for-visual-studio)
- [Technology](#technology)
- [Future Improvements](#future-improvements)
- [Sources](#sources)

## Feature Demo
### Camera Controls and Navigation
<img src="https://github.com/user-attachments/assets/ddf05a7e-e1db-4660-9b3d-041035659d72" width="1500" />

### Model Interaction and Switching
![alien](https://github.com/user-attachments/assets/ec6dc7a4-d810-487f-a763-97fd04b8f6e8)

### Shader Program Selection
![alien](https://github.com/user-attachments/assets/ec6dc7a4-d810-487f-a763-97fd04b8f6e8)

### Light Caster Settings
![alien](https://github.com/user-attachments/assets/ec6dc7a4-d810-487f-a763-97fd04b8f6e8)

### Point Light Configuration
![alien](https://github.com/user-attachments/assets/ec6dc7a4-d810-487f-a763-97fd04b8f6e8)

## Installation
### MacOS

To run this program on MacOS you need to have XCode Command line tools installed
~~~
xcode-select --install
~~~

Then install CMake using [Homebrew](https://brew.sh)
~~~
brew install cmake
~~~

Then clone the github repo onto your computer
~~~
git clone https://github.com/thomasspina/Superchip-emulator.git
~~~

In the repo make a build directory in which the project will be built and switch to that directory
~~~
mkdir build && cd ./build
~~~

From within this build directory you can build the project using cmake and make with the following command
~~~
cmake .. && make -j 4
~~~

Then to run the project you have to run the generated binary in the bin directory within the build directory
~~~
./bin/Superchip-emulator
~~~

### Windows

Install Cmake (PATH recommended) (https://cmake.org/download/)

Then clone the github repo onto your computer
~~~
git clone https://github.com/thomasspina/Superchip-emulator.git
~~~

#### Choose one of the following toolchains:
  - **MinGW**: Install via [MSYS2](https://www.msys2.org/) or the standalone MinGW installer.
  - **Visual Studio**: Install Visual Studio 2019+ with "Desktop Development with C++" workload.

Within the root directory, run the following commands based on the chosen toolchain:

#### For MinGW
~~~
cmake -S . -B build -G "MinGW Makefiles"
mingw32-make -C build
~~~

#### For Visual Studio
~~~
cmake -S . -B build -G "Visual Studio 17 2022"
msbuild build/Superchip-emulator.sln /p:Configuration=Release
~~~

#### Lastly

Run the generated binary in the bin directory within the build directory
~~~
./bin/Superchip-emulator
~~~

## Technology

C++ was chosen as the core language for this project due to its low-level memory control and high performance, which are crucial for optimizing interactions with OpenGL and integrating within a game engine. Furthermore, when paired with CMake, C++ projects can be easily deployed across various systems, ensuring excellent cross-platform compatibility with all major operating systems. 

#### OpenGL

#### GLSL

#### SDL2
[SDL2](https://www.libsdl.org/) is the graphics, sound and periferals library chosen since it is lightweight and had all the basics required for a Chip8 system which isn't super complex to start with.

#### ImGui

#### Assimp

## Future Improvements

This project is the final evolution of the chip8. Any improvements would simply be another system.

## Sources
Technical references for Chip8: 
  - https://multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/ 
  - http://devernay.free.fr/hacks/chip8/C8TECH10.HTM
Technical references for Super-CHIP:
  - https://chip-8.github.io/extensions/#super-chip-10
  - http://devernay.free.fr/hacks/chip8/schip.txt

Super-CHIP and Chip8 Game Library:
  - https://www.zophar.net/pdroms/chip8/super-chip-games-pack.html
  - https://archive.org/details/chip-8-games
