# Quick Start Guide - Cross-Platform MU Online Client

Get up and running in 5 minutes!

## Installation

### Windows 11

```cmd
# 1. Install prerequisites
#    - Visual Studio 2022 (C++ workload)
#    - .NET 9 SDK from https://dotnet.microsoft.com/download/dotnet/9.0
#    - CMake from https://cmake.org/download/
#    - Boost 1.75.0 at C:\Libraries\boost_1_75_0

# 2. Clone and build
git clone https://github.com/yourusername/MuMain.git
cd MuMain
build-windows.bat

# 3. Run
cd build\bin\Release
main.exe
```

### macOS (Your Current Platform!)

```bash
# 1. Install prerequisites
xcode-select --install
brew install cmake glfw openal-soft jpeg-turbo
brew install --cask dotnet-sdk

# 2. Clone and build
git clone https://github.com/yourusername/MuMain.git
cd MuMain
./build-unix.sh

# 3. Run
cd build/bin
./main
```

### Linux (Ubuntu/Debian)

```bash
# 1. Install prerequisites
sudo apt-get update
sudo apt-get install build-essential cmake git
sudo apt-get install libgl1-mesa-dev libglu1-mesa-dev libglfw3-dev
sudo apt-get install libopenal-dev libjpeg-dev

# Install .NET 9 SDK
wget https://dot.net/v1/dotnet-install.sh
chmod +x dotnet-install.sh
./dotnet-install.sh --channel 9.0
export PATH=$PATH:$HOME/.dotnet

# 2. Clone and build
git clone https://github.com/yourusername/MuMain.git
cd MuMain
./build-unix.sh

# 3. Run
cd build/bin
./main
```

## Build Options

### Windows

```cmd
# Debug build
build-windows.bat --config Debug

# x64 build
build-windows.bat --arch x64

# Clean build
build-windows.bat --clean

# Custom Boost path
build-windows.bat --boost "D:\boost_1_75_0"
```

### Linux / macOS

```bash
# Debug build
./build-unix.sh --type Debug

# Clean build
./build-unix.sh --clean

# Specific architecture
./build-unix.sh --arch arm64  # macOS Apple Silicon
```

## Connect to Server

```bash
# Default (localhost:44406)
./main

# Custom server
./main /u192.168.0.20 /p55902

# OpenMU server
./main /uopenmumain.example.com /p55902
```

## Project Structure

```
MuMain/
├── build/                    # Build output (generated)
│   └── bin/                 # Executables here
├── ClientLibrary/           # C# networking (.NET 9)
├── Source Main 5.2/         # C++ client code
│   ├── source/              # Main source files
│   │   ├── Platform/       # ✨ NEW: Cross-platform abstractions
│   │   ├── Winmain.cpp     # Entry point
│   │   └── ...
│   └── dependencies/        # Third-party libs
├── BUILD.md                 # 📖 Detailed build guide
├── CROSSPLATFORM.md         # 🏗️ Implementation guide
├── CHANGES.md               # 📝 What changed
└── QUICKSTART.md            # ⚡ This file
```

## Common Issues

### Windows: Boost not found
```cmd
# Install to C:\Libraries\boost_1_75_0 or specify path:
build-windows.bat --boost "C:\path\to\boost"
```

### macOS: Permission denied on build script
```bash
chmod +x build-unix.sh
./build-unix.sh
```

### Linux: .NET not found
```bash
# Add to PATH
export PATH=$PATH:$HOME/.dotnet
# Add to ~/.bashrc to make permanent
echo 'export PATH=$PATH:$HOME/.dotnet' >> ~/.bashrc
```

### All Platforms: CMake not found
- Windows: Reinstall CMake and check "Add to PATH"
- macOS: `brew install cmake`
- Linux: `sudo apt-get install cmake`

## What Works Now

| Feature | Status | Notes |
|---------|--------|-------|
| ✅ Build System | **Ready** | CMake for all platforms |
| ✅ C# Library | **Ready** | .NET 9 Native AOT |
| ✅ OpenGL | **Ready** | Graphics rendering |
| ⚠️ Windowing | **Partial** | Windows native works, GLFW ready |
| ⚠️ Input | **Partial** | Windows native works, abstraction ready |
| ⚠️ Audio | **Partial** | Windows DirectSound works, OpenAL ready |
| ✅ Networking | **Ready** | Cross-platform C# |

## Next Steps for Developers

1. **Test Windows build first** (easiest to verify)
2. **Implement GLFW window** (`PlatformWindow_GLFW.cpp`)
3. **Implement GLFW input** (`PlatformInput_GLFW.cpp`)
4. **Implement OpenAL audio** (`PlatformAudio_OpenAL.cpp`)
5. **Test on Linux and macOS**

See **[CROSSPLATFORM.md](CROSSPLATFORM.md)** for detailed implementation guide.

## Resources

- 📖 **Detailed Build Guide:** [BUILD.md](BUILD.md)
- 🏗️ **Implementation Guide:** [CROSSPLATFORM.md](CROSSPLATFORM.md)
- 📝 **Changes Summary:** [CHANGES.md](CHANGES.md)
- 🐛 **Issues:** https://github.com/yourusername/MuMain/issues
- 🎮 **Server (OpenMU):** https://github.com/MUnique/OpenMU

## Quick Commands Reference

| Task | Windows | Linux/macOS |
|------|---------|-------------|
| **Build** | `build-windows.bat` | `./build-unix.sh` |
| **Clean** | `build-windows.bat --clean` | `./build-unix.sh --clean` |
| **Debug** | `build-windows.bat --config Debug` | `./build-unix.sh --type Debug` |
| **Run** | `build\bin\Release\main.exe` | `build/bin/main` |

## Need Help?

1. Check [BUILD.md](BUILD.md) for detailed build instructions
2. Check [CROSSPLATFORM.md](CROSSPLATFORM.md) for architecture info
3. Search existing issues on GitHub
4. Create a new issue with:
   - Your OS and version
   - Error message
   - Steps to reproduce

---

**Ready to build?** Pick your platform above and follow the 3-step process!
