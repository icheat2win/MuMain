#!/bin/bash
# Test script to verify the cross-platform entry point compiles

set -e

echo "=== Testing Cross-Platform Integration ==="
echo ""

# Check for GLFW
if ! pkg-config --exists glfw3; then
    echo "ERROR: GLFW not found"
    echo "Install with: brew install glfw"
    exit 1
fi

echo "✓ Found GLFW: $(pkg-config --modversion glfw3)"

# Get GLFW flags
GLFW_CFLAGS=$(pkg-config --cflags glfw3)
GLFW_LIBS=$(pkg-config --libs glfw3)

# Compiler flags (without paths that have spaces)
CXXFLAGS="-std=c++17 -DUSE_GLFW=1 -DUSE_CROSSPLATFORM_MAIN=1 -DPLATFORM_MACOS=1 -DPLATFORM_WINDOWS=0 -DPLATFORM_LINUX=0 -DGL_SILENCE_DEPRECATION"

# macOS frameworks
FRAMEWORKS="-framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo"

echo "Compiling cross-platform entry point..."
echo ""

clang++ $CXXFLAGS \
    -I"Source Main 5.2/source" \
    "Source Main 5.2/source/main_crossplatform.cpp" \
    "Source Main 5.2/source/Platform/Platform.cpp" \
    "Source Main 5.2/source/Platform/PlatformWindow_GLFW.cpp" \
    $GLFW_CFLAGS \
    $GLFW_LIBS \
    $FRAMEWORKS \
    -o test_integration

if [ $? -eq 0 ]; then
    echo "✓ Compilation successful!"
    echo ""
    echo "Run with: ./test_integration"
    echo "Or with server: ./test_integration connect /u192.168.0.20 /p55902"
    echo ""
else
    echo "✗ Compilation failed"
    exit 1
fi
