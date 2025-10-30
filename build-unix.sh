#!/bin/bash
# Build script for MU Online Client on Linux and macOS
# This script automates the CMake configuration and build process

set -e  # Exit on error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Default values
BUILD_DIR="build"
BUILD_TYPE="Release"
ARCHITECTURE=""
CLEAN=0

# Function to print colored headers
print_header() {
    echo -e "\n${CYAN}================================================${NC}"
    echo -e "${CYAN}$1${NC}"
    echo -e "${CYAN}================================================${NC}"
}

# Function to print success messages
print_success() {
    echo -e "${GREEN}$1${NC}"
}

# Function to print error messages
print_error() {
    echo -e "${RED}$1${NC}"
}

# Function to print warning messages
print_warning() {
    echo -e "${YELLOW}$1${NC}"
}

# Show help
show_help() {
    echo "MU Online Client - Unix Build Script"
    echo ""
    echo "Usage: ./build-unix.sh [options]"
    echo ""
    echo "Options:"
    echo "  --type TYPE        Build type: Debug or Release (default: Release)"
    echo "  --arch ARCH        Architecture: x86_64 or arm64 (auto-detect if not specified)"
    echo "  --clean            Clean build directory before building"
    echo "  --help             Show this help message"
    echo ""
    echo "Examples:"
    echo "  ./build-unix.sh --type Debug"
    echo "  ./build-unix.sh --clean --type Release"
    echo "  ./build-unix.sh --arch arm64"
    exit 0
}

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        --type)
            BUILD_TYPE="$2"
            shift 2
            ;;
        --arch)
            ARCHITECTURE="$2"
            shift 2
            ;;
        --clean)
            CLEAN=1
            shift
            ;;
        --help)
            show_help
            ;;
        *)
            print_error "Unknown option: $1"
            echo "Use --help for usage information"
            exit 1
            ;;
    esac
done

print_header "MU Online Client - Unix Build Script"

# Detect platform
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    PLATFORM="Linux"
elif [[ "$OSTYPE" == "darwin"* ]]; then
    PLATFORM="macOS"
else
    print_error "Unsupported platform: $OSTYPE"
    exit 1
fi

# Auto-detect architecture if not specified
if [ -z "$ARCHITECTURE" ]; then
    ARCHITECTURE=$(uname -m)
fi

echo "Platform: $PLATFORM"
echo "Architecture: $ARCHITECTURE"
echo "Build Type: $BUILD_TYPE"
echo "Build Directory: $BUILD_DIR"
echo ""

# Check for required tools
print_header "Checking Prerequisites"

# Check for CMake
if ! command -v cmake &> /dev/null; then
    print_error "ERROR: cmake not found"
    echo "Please install CMake:"
    if [[ "$PLATFORM" == "Linux" ]]; then
        echo "  Ubuntu/Debian: sudo apt-get install cmake"
        echo "  Fedora: sudo dnf install cmake"
        echo "  Arch: sudo pacman -S cmake"
    else
        echo "  macOS: brew install cmake"
    fi
    exit 1
fi
print_success "✓ Found CMake: $(cmake --version | head -n 1)"

# Check for dotnet
if ! command -v dotnet &> /dev/null; then
    print_error "ERROR: dotnet not found"
    echo "Please install .NET 9 SDK from:"
    echo "  https://dotnet.microsoft.com/download/dotnet/9.0"
    exit 1
fi
DOTNET_VERSION=$(dotnet --version)
print_success "✓ Found .NET: $DOTNET_VERSION"

# Verify .NET version
if [[ ! "$DOTNET_VERSION" =~ ^9\. ]]; then
    print_warning "WARNING: .NET 9 SDK not detected"
    echo "Current version: $DOTNET_VERSION"
    echo "Required: 9.0 or later"
fi

# Check for build tools
if [[ "$PLATFORM" == "Linux" ]]; then
    if ! command -v gcc &> /dev/null && ! command -v clang &> /dev/null; then
        print_error "ERROR: No C++ compiler found (gcc or clang)"
        echo "Please install build tools:"
        echo "  Ubuntu/Debian: sudo apt-get install build-essential"
        echo "  Fedora: sudo dnf groupinstall 'Development Tools'"
        echo "  Arch: sudo pacman -S base-devel"
        exit 1
    fi
else
    if ! command -v clang &> /dev/null; then
        print_error "ERROR: Xcode Command Line Tools not found"
        echo "Please install: xcode-select --install"
        exit 1
    fi
fi
print_success "✓ Found C++ compiler"

# Check for GLFW
if pkg-config --exists glfw3; then
    print_success "✓ Found GLFW: $(pkg-config --modversion glfw3)"
else
    print_warning "⚠ GLFW not found (optional - will use native windowing)"
    echo "To install GLFW:"
    if [[ "$PLATFORM" == "Linux" ]]; then
        echo "  Ubuntu/Debian: sudo apt-get install libglfw3-dev"
        echo "  Fedora: sudo dnf install glfw-devel"
        echo "  Arch: sudo pacman -S glfw-x11"
    else
        echo "  macOS: brew install glfw"
    fi
fi

# Check for OpenAL
if pkg-config --exists openal; then
    print_success "✓ Found OpenAL: $(pkg-config --modversion openal)"
elif [[ "$PLATFORM" == "macOS" ]]; then
    print_success "✓ OpenAL (included with macOS)"
else
    print_warning "⚠ OpenAL not found (optional - will use platform audio)"
    echo "To install OpenAL:"
    echo "  Ubuntu/Debian: sudo apt-get install libopenal-dev"
    echo "  Fedora: sudo dnf install openal-soft-devel"
    echo "  Arch: sudo pacman -S openal"
fi

# Check for OpenGL
if [[ "$PLATFORM" == "Linux" ]]; then
    if pkg-config --exists gl; then
        print_success "✓ Found OpenGL"
    else
        print_warning "⚠ OpenGL development libraries not found"
        echo "To install:"
        echo "  Ubuntu/Debian: sudo apt-get install libgl1-mesa-dev libglu1-mesa-dev"
        echo "  Fedora: sudo dnf install mesa-libGL-devel mesa-libGLU-devel"
        echo "  Arch: sudo pacman -S mesa glu"
    fi
else
    print_success "✓ OpenGL (included with macOS)"
fi

echo ""

# Clean if requested
if [ $CLEAN -eq 1 ] && [ -d "$BUILD_DIR" ]; then
    print_warning "Cleaning build directory..."
    rm -rf "$BUILD_DIR"
fi

# Create build directory
if [ ! -d "$BUILD_DIR" ]; then
    echo "Creating build directory..."
    mkdir -p "$BUILD_DIR"
fi

cd "$BUILD_DIR"

# Configure with CMake
print_header "Step 1: Configuring with CMake"
cmake .. -DCMAKE_BUILD_TYPE="$BUILD_TYPE"
if [ $? -ne 0 ]; then
    print_error "ERROR: CMake configuration failed"
    exit 1
fi
print_success "Configuration successful!"

# Build C# Library
print_header "Step 2: Building C# Library (Native AOT)"
cmake --build . --config "$BUILD_TYPE" --target ClientLibrary
if [ $? -ne 0 ]; then
    print_error "ERROR: C# library build failed"
    exit 1
fi
print_success "C# library built successfully!"

# Build C++ Executable
print_header "Step 3: Building C++ Main Executable"
cmake --build . --config "$BUILD_TYPE" --target main -- -j$(nproc 2>/dev/null || sysctl -n hw.ncpu)
if [ $? -ne 0 ]; then
    print_error "ERROR: C++ build failed"
    exit 1
fi
print_success "C++ executable built successfully!"

cd ..

print_header "Build Successful!"
echo ""
print_success "Output location: $BUILD_DIR/bin/"
print_success "Executable: main"
echo ""
echo "To run the client:"
echo "  cd $BUILD_DIR/bin"
echo "  ./main"
echo ""
echo "Or with server connection:"
echo "  ./main /u192.168.0.20 /p55902"
echo ""
