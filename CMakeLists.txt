cmake_minimum_required(VERSION 3.25)

project(MuMain VERSION 5.2 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Platform detection
if(WIN32)
    set(PLATFORM_WINDOWS TRUE)
elseif(UNIX AND NOT APPLE)
    set(PLATFORM_LINUX TRUE)
elseif(APPLE)
    set(PLATFORM_MACOS TRUE)
endif()

# Build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()

# Options
option(BUILD_CLIENT_LIBRARY "Build the C# client library" ON)
option(BUILD_MAIN_EXECUTABLE "Build the main C++ executable" ON)

# Global settings
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Dependencies paths (can be overridden by user)
if(PLATFORM_WINDOWS)
    set(BOOST_ROOT "C:/Libraries/boost_1_75_0" CACHE PATH "Boost installation path")
endif()

# Find required packages
message(STATUS "Searching for dependencies...")

# OpenGL (required for all platforms)
find_package(OpenGL REQUIRED)
if(OPENGL_FOUND)
    message(STATUS "Found OpenGL: ${OPENGL_LIBRARIES}")
endif()

# GLFW (cross-platform window management)
find_package(glfw3 QUIET)
if(glfw3_FOUND)
    message(STATUS "Found GLFW: ${glfw3_VERSION}")
    set(USE_GLFW ON CACHE BOOL "Use GLFW for window management")
else()
    message(STATUS "GLFW not found - will use native window management")
    set(USE_GLFW OFF CACHE BOOL "Use GLFW for window management")
endif()

# OpenAL (cross-platform audio)
find_package(OpenAL QUIET)
if(OPENAL_FOUND)
    message(STATUS "Found OpenAL: ${OPENAL_LIBRARY}")
    set(USE_OPENAL ON CACHE BOOL "Use OpenAL for audio")
else()
    message(STATUS "OpenAL not found - will use platform-specific audio")
    set(USE_OPENAL OFF CACHE BOOL "Use OpenAL for audio")
endif()

# JPEG library (cross-platform)
find_package(JPEG QUIET)
if(JPEG_FOUND)
    message(STATUS "Found JPEG: ${JPEG_LIBRARIES}")
endif()

# Add subdirectories
if(BUILD_CLIENT_LIBRARY)
    add_subdirectory(ClientLibrary)
endif()

if(BUILD_MAIN_EXECUTABLE)
    add_subdirectory("Source Main 5.2")
endif()

# Installation
if(PLATFORM_MACOS)
    install(TARGETS main
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        BUNDLE DESTINATION .
    )
else()
    install(TARGETS main
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
    )
endif()
