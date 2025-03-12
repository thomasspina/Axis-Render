cmake_minimum_required(VERSION 3.22)
project(Obj-renderer)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

##############################
# Make and Compiler settings #
##############################

# Enable parallel build
include(ProcessorCount)
ProcessorCount(N)
if(NOT N EQUAL 0)
    set(CMAKE_BUILD_PARALLEL_LEVEL ${N})
endif()

# Build type configuration
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()

# Optimize compiler flags
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -march=native -flto")
    elseif(MSVC)
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2 /GL /Gy")
        set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /LTCG")
    endif()
endif()

# Suppress warnings about unknown pragmas
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-error=unknown-pragmas")
endif()




#####################
# External libraries #
#####################

# Find required packages
find_package(OpenGL REQUIRED)

include(FetchContent)
FetchContent_Declare(SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-2.30.x
    GIT_SHALLOW ON
    SYSTEM
)

FetchContent_Declare(imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG master
    GIT_SHALLOW ON
    SYSTEM
)

FetchContent_Declare(assimp
    GIT_REPOSITORY https://github.com/assimp/assimp.git
    GIT_TAG v5.4.3
    GIT_SHALLOW ON
)

FetchContent_Declare(glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.1
    GIT_SHALLOW ON
    SYSTEM   
)

FetchContent_Declare(GLEW
    GIT_REPOSITORY https://github.com/Perlmint/glew-cmake.git
    GIT_TAG glew-cmake-2.2.0
    GIT_SHALLOW ON
    SYSTEM
)

# GLEW build options
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Disable shared libs" FORCE)
set(GLEW_BUILD_SHARED OFF CACHE BOOL "Disable GLEW shared" FORCE)
set(GLEW_BUILD_STATIC ON CACHE BOOL "Enable GLEW static" FORCE)
set(GLEW_BUILD_UTILS OFF CACHE BOOL "Disable GLEW utils" FORCE)
set(GLEW_OSMESA OFF CACHE BOOL "Disable OSMesa" FORCE)
set(GLEW_EGL OFF CACHE BOOL "Disable EGL" FORCE)

# SDL2 build options
set(SDL_TIMERS ON CACHE BOOL "Enable SDL timer subsystem" FORCE)
set(SDL_VIDEO ON CACHE BOOL "Enable SDL video subsystem" FORCE)
set(SDL_EVENTS ON CACHE BOOL "Enable SDL events subsystem" FORCE)
set(SDL_THREADS ON CACHE BOOL "Enable SDL threads - required for timers" FORCE)
set(SDL_OPENGL ON CACHE BOOL "Enable OpenGL" FORCE)
set(SDL_X11 ON CACHE BOOL "Enable X11" FORCE)

set(SDL_TEST OFF CACHE BOOL "Enable SDL test framework compilation" FORCE)
set(SDL_RENDER OFF CACHE BOOL "Enable SDL rendering subsystem" FORCE)
set(SDL_JOYSTICK OFF CACHE BOOL "Enable SDL joystick subsystem" FORCE)
set(SDL_HAPTIC OFF CACHE BOOL "Enable SDL haptic feedback subsystem" FORCE)
set(SDL_HIDAPI OFF CACHE BOOL "Enable SDL HIDAPI support" FORCE)
set(SDL_POWER OFF CACHE BOOL "Enable SDL power management subsystem" FORCE)
set(SDL_AUDIO OFF CACHE BOOL "Enable SDL audio subsystem" FORCE)
set(SDL_ATOMIC OFF CACHE BOOL "Enable SDL atomic operations" FORCE)
set(SDL_FILE OFF CACHE BOOL "Enable SDL file I/O abstractions" FORCE)
set(SDL_CPUINFO OFF CACHE BOOL "Enable SDL CPU information functions" FORCE)
set(SDL_FILESYSTEM OFF CACHE BOOL "Enable SDL filesystem functions" FORCE)
set(SDL_SENSOR OFF CACHE BOOL "Enable SDL sensor subsystem" FORCE)
set(SDL_LOCALE OFF CACHE BOOL "Enable SDL locale subsystem" FORCE)
set(SDL_MISC OFF CACHE BOOL "Enable SDL miscellaneous subsystems" FORCE)
set(SDL_OPENGLES OFF CACHE BOOL "Disable OpenGL ES" FORCE)
set(SDL_VULKAN OFF CACHE BOOL "Disable Vulkan" FORCE)
set(SDL_WAYLAND OFF CACHE BOOL "Disable Wayland" FORCE)
set(SDL_WAYLAND_SHARED OFF CACHE BOOL "Disable Wayland shared" FORCE)
set(SDL_WAYLAND_LIBDECOR OFF CACHE BOOL "Disable Wayland libdecor" FORCE)
set(SDL_WAYLAND_LIBDECOR_SHARED OFF CACHE BOOL "Disable Wayland libdecor shared" FORCE)
set(SDL_WAYLAND_QT_TOUCH OFF CACHE BOOL "Disable Wayland Qt touch" FORCE)
set(SDL_KMSDRM OFF CACHE BOOL "Disable KMS/DRM" FORCE)
set(SDL_KMSDRM_SHARED OFF CACHE BOOL "Disable KMS/DRM shared" FORCE)
set(SDL_ALSA OFF CACHE BOOL "Disable ALSA" FORCE)
set(SDL_ALSA_SHARED OFF CACHE BOOL "Disable ALSA shared" FORCE)
set(SDL_ARTS OFF CACHE BOOL "Disable ARTS" FORCE)
set(SDL_ARTS_SHARED OFF CACHE BOOL "Disable ARTS shared" FORCE)
set(SDL_ESD OFF CACHE BOOL "Disable ESD" FORCE)
set(SDL_ESD_SHARED OFF CACHE BOOL "Disable ESD shared" FORCE)
set(SDL_JACK OFF CACHE BOOL "Disable JACK" FORCE)
set(SDL_JACK_SHARED OFF CACHE BOOL "Disable JACK shared" FORCE)
set(SDL_NAS OFF CACHE BOOL "Disable NAS" FORCE)
set(SDL_NAS_SHARED OFF CACHE BOOL "Disable NAS shared" FORCE)
set(SDL_OSS OFF CACHE BOOL "Disable OSS" FORCE)
set(SDL_PIPEWIRE OFF CACHE BOOL "Disable PipeWire" FORCE)
set(SDL_PIPEWIRE_SHARED OFF CACHE BOOL "Disable PipeWire shared" FORCE)
set(SDL_PULSEAUDIO OFF CACHE BOOL "Disable PulseAudio" FORCE)
set(SDL_PULSEAUDIO_SHARED OFF CACHE BOOL "Disable PulseAudio shared" FORCE)
set(SDL_SNDIO OFF CACHE BOOL "Disable SNDIO" FORCE)
set(SDL_SNDIO_SHARED OFF CACHE BOOL "Disable SNDIO shared" FORCE)


# Assimp build options
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "Disable Assimp tests" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "Disable Assimp tools" FORCE)
set(ASSIMP_BUILD_SAMPLES OFF CACHE BOOL "Disable Assimp samples" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "Disable Assimp install" FORCE)
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "Don't build all importers" FORCE)
set(ASSIMP_BUILD_OBJ_IMPORTER ON CACHE BOOL "Build OBJ importer" FORCE) # only have OBJ, FBX and GLTF importers
set(ASSIMP_BUILD_FBX_IMPORTER ON CACHE BOOL "Build FBX importer" FORCE) 
set(ASSIMP_BUILD_GLTF_IMPORTER ON CACHE BOOL "Build GLTF importer" FORCE)
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF CACHE BOOL "Don't build exporters" FORCE)

FetchContent_MakeAvailable(SDL2 imgui assimp glm GLEW)

# Safeguard for GLEW. If the target is shared, don't build it
if (TARGET libglew_shared)
    set_target_properties(libglew_shared PROPERTIES EXCLUDE_FROM_ALL TRUE)
endif()

# ImGui library target setup
add_library(ImGui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
)
target_include_directories(ImGui PUBLIC
    ${imgui_SOURCE_DIR}
)
add_library(ImGui-SDL2 STATIC
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl2.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)
target_include_directories(ImGui-SDL2 PUBLIC
    ${imgui_SOURCE_DIR}             # Ensure ImGui core headers
    ${imgui_SOURCE_DIR}/backends    # Ensure SDL2 and OpenGL backends
    ${SDL2_SOURCE_DIR}/include 
)


# Get stb_image header file
set(STB_IMAGE_URL "https://raw.githubusercontent.com/nothings/stb/master/stb_image.h")
set(STB_IMAGE_PATH "${CMAKE_BINARY_DIR}/stb_include/stb_image.h")
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/stb_include")
file(DOWNLOAD ${STB_IMAGE_URL} ${STB_IMAGE_PATH})

# stb library
add_library(stb_image INTERFACE)
target_include_directories(stb_image INTERFACE "${CMAKE_BINARY_DIR}/stb_include")


#####################
# Project settings #
#####################


# Source files
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp)
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

# Configure where assets are, for the shaders, models, etc
set(ASSETS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/assets/")
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/config.h.in
    ${CMAKE_BINARY_DIR}/config.h
)

# Create executable
add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})

# Include directories
target_include_directories(${PROJECT_NAME}
    PRIVATE 
        ${GLEW_INCLUDE_DIR}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_BINARY_DIR}
)

# GLEW_STATIC preprocessing macro is needed for static linking
target_compile_definitions(${PROJECT_NAME} PRIVATE GLEW_STATIC) 

# Link libraries
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        SDL2::SDL2-static
        SDL2::SDL2main
        ImGui-SDL2
        ImGui
        OpenGL::GL
        libglew_static
        glm
        assimp
)


target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)

# Platform-specific settings
if(WIN32)
 target_link_libraries(${PROJECT_NAME} PRIVATE opengl32)
endif()
