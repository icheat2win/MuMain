#!/bin/bash
# Build script for platform test program

set -e

echo "=== Building Platform Test ==="
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

# Compiler flags
CXXFLAGS="-std=c++17 -I. -DUSE_GLFW=1 -DPLATFORM_MACOS=1 -DPLATFORM_WINDOWS=0 -DPLATFORM_LINUX=0 -DGL_SILENCE_DEPRECATION"

# macOS frameworks
FRAMEWORKS="-framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo"

echo "Compiling test_platform..."

clang++ $CXXFLAGS \
    test_platform.cpp \
    $GLFW_CFLAGS \
    $GLFW_LIBS \
    $FRAMEWORKS \
    -o test_platform

if [ $? -eq 0 ]; then
    echo ""
    echo "✓ Build successful!"
    echo ""
    echo "Run with: ./test_platform"
    echo ""
else
    echo "✗ Build failed"
    exit 1
fi
