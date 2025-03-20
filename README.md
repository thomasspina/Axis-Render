# Axis Render Engine

Axis Render is a rasterization-based 3D model rendering engine designed to visualize OBJ models. This project serves as an initial step in developing a game engine, providing the foundational tools for rendering and interacting with 3D models. As a standalone application, Axis Render offers a straightforward UI that allows users to manipulate and explore 3D models in real-time. With features like free-roam camera movement, real-time shader application, dynamic lighting adjustments, and model transformations, it provides a flexible platform for visualizing and fine-tuning 3D assets, laying the groundwork for more advanced game engine development.

## Navigation
- [Features](#features)
- [Installation](#installation)
  - [MacOS](#macos)
  - [Windows](#windows)
    - [MinGW](#for-mingw)
    - [Visual Studio](#for-visual-studio)
- [Technology](#technology)
- [Future Improvements](#future-improvements)
- [Sources](#sources)

## Features
### Model Interaction
- **Two Rotation Modes**:
  - **Input Rotation**: Control the model's rotation using mouse movement.
  - **Natural Rotation**: The model rotates smoothly in a continuous diagonal motion.

- **Adjust Model Scale**: Change the size of the model to fit your preference.

- **Select Model**: Choose the model you want to render.

- **Select Shader**: Apply custom shaders to enhance the model's visual appearance.

- **Grid**: A grid can be toggled to provide a better perspective in 3D space.

<img src="https://github.com/user-attachments/assets/8141882e-8cd8-4b60-9c14-146676f82e28" width="1500" />

---

### Camera Controls and Navigation
- **Toggleable Camera Options**:
  - **Camera Rotation**: Enables users to rotate the camera view from a first-person point of view (POV).
  - **Free Roam Camera**: Allows the user to freely fly around the 3D space.

- **Camera Zoom**: Users can zoom in and out for a closer or wider view of the scene.
<img src="https://github.com/user-attachments/assets/1be77441-b612-473a-a2cd-421bb1e9c471" width="1500" />

---

### Shader Programs 
Several shader programs are currently available for selection, with more being developed and updated over time. Below are shaders that stand out a little more.

#### Phong
The Phong shading model is a technique used in computer graphics to simulate the way light interacts with surfaces. It is widely used in rasterization engines for real-time rendering and produces smooth lighting effects. The model combines three components:

Ambient Lighting: A constant light that affects all surfaces equally, providing a baseline level of light.

Diffuse Reflection: Simulates how light scatters on a rough surface, resulting in a matte finish.

Specular Reflection: Models the shiny highlights on smooth surfaces where light reflects at specific angles, contributing to the glossy appearance.

<img src="https://github.com/user-attachments/assets/75deaca9-6a12-4d71-9c40-62b3f7ed5b35" width="1500" />

#### ASCII
The ASCII Shader converts 3D models or images into stylized ASCII art by using a bitmask pattern that is selected based on the pixel's brightness. 

The shader first computes the diffuse and specular lighting, which contribute to the final pixel brightness. This 
brightness is then used to choose a bitmask, which encodes a 5x5 character grid. The shader scales and translates the pixel's screen position into this grid, checking whether each grid cell should be "on" or "off" based on the bitmask. 

The result is a pixelated output that simulates ASCII characters, effectively transforming the 3D model into a low-resolution, text-based visual representation.

This shader was created from this [tutorial](https://tympanus.net/codrops/2024/11/13/creating-an-ascii-shader-using-ogl/).

<img src="https://github.com/user-attachments/assets/771b8e45-1e96-4e1c-bc2d-48221804fd98" width="1500" />

---

### Light Caster Settings

### Point Light Configuration

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

OpenGL was chosen as the graphics API due to its widespread use and lower hardware requirements. This ensures that our engine can run efficiently on a wide range of devices, including lower-end systems, providing a broader accessibility to users with varying hardware capabilities.

#### GLSL

GLSL (OpenGL Shading Language) is the standard shader language used for OpenGL applications, enabling efficient and flexible GPU programming for rendering tasks.

#### SDL2
[SDL2](https://www.libsdl.org/) was used to create and manage the application window and OpenGL context. It is a easy-to-use library that not only streamlines development but also allows for seamless portability to other platforms, including WebAssembly (WASM), if needed.

#### ImGui
[ImGui](https://github.com/ocornut/imgui) was used for the UI due to its lightweight nature and straightforward approach to creating complex UIs

#### Assimp
[Assimp](https://www.assimp.org/) was used for processing 3D model files. It simplifies the process of importing and converting model data into a format that can be efficiently integrated into the GPU for rendering.

## Future Improvements

Future improvements include expanding the engine's capability to process additional 3D model formats, such as GLTF and FBX, beyond just OBJ files. Other desirable features would include enabling users to modify model coordinates using drag-and-drop functionality, as well as supporting the rendering of multiple objects simultaneously. More flashy shaders would also be neat.

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
