cmake_minimum_required(VERSION 3.22)
project(Obj-renderer)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-error=unknown-pragmas")
endif()

# Find required packages
find_package(OpenGL REQUIRED)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "Disable shared libs" FORCE)
set(GLEW_BUILD_SHARED OFF CACHE BOOL "Disable GLEW shared" FORCE)
set(GLEW_BUILD_STATIC ON CACHE BOOL "Enable GLEW static" FORCE)
set(GLEW_BUILD_UTILS OFF CACHE BOOL "Disable GLEW utils" FORCE)

include(FetchContent)
FetchContent_Declare(SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-2.30.x
    GIT_SHALLOW ON
    SYSTEM
)

FetchContent_Declare(SDL2_image
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
    GIT_TAG release-2.8.x
    GIT_SHALLOW ON
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

FetchContent_Declare(stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG master
    GIT_SHALLOW ON
    SYSTEM
)

# SDL2 build options
set(SDL_TEST OFF CACHE BOOL "" FORCE)
set(SDL_RENDER OFF CACHE BOOL "" FORCE) # TODO: Check if this is necessary
set(SDL_VIDEO ON CACHE BOOL "" FORCE)
set(SDL_JOYSTICK OFF CACHE BOOL "" FORCE)
set(SDL_HAPTIC OFF CACHE BOOL "" FORCE)
set(SDL_HIDAPI OFF CACHE BOOL "" FORCE)
set(SDL_POWER OFF CACHE BOOL "" FORCE)

# Disable Assimp extras
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "Disable Assimp tests" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "Disable Assimp tools" FORCE)
set(ASSIMP_BUILD_SAMPLES OFF CACHE BOOL "Disable Assimp samples" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "Disable Assimp install" FORCE)

FetchContent_MakeAvailable(SDL2 SDL2_image assimp glm GLEW stb)

# stb library
add_library(stb_image INTERFACE)
target_include_directories(stb_image INTERFACE ${stb_SOURCE_DIR})

if (TARGET libglew_shared)
    set_target_properties(libglew_shared PROPERTIES EXCLUDE_FROM_ALL TRUE)
endif()

# Source files
file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp)
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

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


# Link libraries
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        SDL2::SDL2-static
        SDL2::SDL2main
        SDL2_image
        OpenGL::GL
        libglew_static
        glm
        assimp
        stb_image
)

target_compile_definitions(${PROJECT_NAME} PRIVATE GLEW_STATIC)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)

# Platform-specific settings
if(WIN32)
 target_link_libraries(${PROJECT_NAME} PRIVATE opengl32)
endif()
