#!/bin/bash
# Build script for simple main test (without game headers)

set -e

echo "=== Building Simple Main Test ==="
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
CXXFLAGS="-std=c++17 -DGL_SILENCE_DEPRECATION"

# macOS frameworks
FRAMEWORKS="-framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo"

echo "Compiling simple main test..."
echo ""

# Compile all files separately to avoid include path issues
echo "Compiling with GameCore..."
clang++ $CXXFLAGS \
    -DUSE_GLFW=1 \
    -DUSE_CROSSPLATFORM_MAIN=1 \
    "Source Main 5.2/source/main_crossplatform.cpp" \
    "Source Main 5.2/source/GameCore.cpp" \
    "Source Main 5.2/source/GameGlobals.cpp" \
    "Source Main 5.2/source/Platform/Platform.cpp" \
    "Source Main 5.2/source/Platform/PlatformWindow_GLFW.cpp" \
    -I"Source Main 5.2/source" \
    $GLFW_CFLAGS \
    $GLFW_LIBS \
    $FRAMEWORKS \
    -o main_crossplatform

if [ $? -eq 0 ]; then
    echo ""
    echo "✓ Compilation successful!"
    echo ""
    echo "Run with: ./main_crossplatform"
    echo "Or with server: ./main_crossplatform connect /u192.168.0.20 /p55902"
    echo ""
else
    echo "✗ Compilation failed"
    exit 1
fi
