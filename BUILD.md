# Building MU Online Client

This document provides instructions for building the MU Online client on Windows 11, Linux, and macOS.

## Current Status

- ✅ **Windows 11**: Build system configured (CMake + Visual Studio)
- 🚧 **Linux**: Planned (requires cross-platform abstractions)
- 🚧 **macOS**: Planned (requires cross-platform abstractions)

---

## Prerequisites

### All Platforms

1. **CMake** 3.25 or later
   - Download: https://cmake.org/download/

2. **.NET 9 SDK**
   - Download: https://dotnet.microsoft.com/download/dotnet/9.0
   - Required for building the C# networking library with Native AOT

3. **Git** (for cloning the repository)

### Windows 11

1. **Visual Studio 2022**
   - Download: https://visualstudio.microsoft.com/downloads/
   - Required workloads:
     - Desktop development with C++
     - .NET desktop development
   - Individual components:
     - MSVC v143 (or latest)
     - Windows 10/11 SDK
     - C++/CLI support

2. **Boost 1.75.0** (or compatible version)
   - Download: https://www.boost.org/users/history/version_1_75_0.html
   - **Installation path**: `C:\Libraries\boost_1_75_0`
     - If installing elsewhere, set `-DBOOST_ROOT=<path>` when running CMake
   - Build Boost with:
     ```cmd
     bootstrap.bat
     b2 --with-system --with-filesystem --with-thread variant=release link=static threading=multi runtime-link=static address-model=32
     ```

3. **OpenGL Development Libraries** (usually included with Windows SDK)

### Linux (Future)

*Coming soon - requires GLFW/SDL2, OpenAL, and other cross-platform libraries*

### macOS (Future)

*Coming soon - requires GLFW/SDL2, OpenAL, and other cross-platform libraries*

---

## Building on Windows 11

### Step 1: Install Prerequisites

1. Install Visual Studio 2022 with the required workloads
2. Install .NET 9 SDK
3. Install CMake (ensure it's added to PATH)
4. Install/Build Boost 1.75.0 at `C:\Libraries\boost_1_75_0`

### Step 2: Clone the Repository

```cmd
git clone https://github.com/yourusername/MuMain.git
cd MuMain
```

### Step 3: Configure with CMake

Open a **Developer Command Prompt for VS 2022** (or PowerShell with VS env):

```cmd
# Create build directory
mkdir build
cd build

# Configure for Win32 (x86) - Default
cmake .. -G "Visual Studio 17 2022" -A Win32

# Or configure for x64
cmake .. -G "Visual Studio 17 2022" -A x64

# If Boost is not at default location:
cmake .. -G "Visual Studio 17 2022" -A Win32 -DBOOST_ROOT="C:\path\to\boost"
```

### Step 4: Build

**Option A: Using CMake**

```cmd
# Debug build
cmake --build . --config Debug

# Release build
cmake --build . --config Release
```

**Option B: Using Visual Studio**

1. Open `MuMain.sln` in the `build` directory
2. Select configuration (Debug/Release)
3. Build > Build Solution (F7)

### Step 5: Run

```cmd
cd bin\Release
main.exe

# Or with server parameters:
main.exe /u192.168.0.20 /p55902
```

---

## Building on Linux

### Prerequisites

1. **Build Tools**
   ```bash
   # Ubuntu/Debian
   sudo apt-get update
   sudo apt-get install build-essential cmake git

   # Fedora
   sudo dnf groupinstall 'Development Tools'
   sudo dnf install cmake git

   # Arch Linux
   sudo pacman -S base-devel cmake git
   ```

2. **.NET 9 SDK**
   ```bash
   # Download and install from Microsoft
   wget https://dot.net/v1/dotnet-install.sh -O dotnet-install.sh
   chmod +x dotnet-install.sh
   ./dotnet-install.sh --channel 9.0

   # Add to PATH (add to ~/.bashrc or ~/.zshrc)
   export PATH=$PATH:$HOME/.dotnet
   ```

3. **Graphics Libraries**
   ```bash
   # Ubuntu/Debian
   sudo apt-get install libgl1-mesa-dev libglu1-mesa-dev libglfw3-dev

   # Fedora
   sudo dnf install mesa-libGL-devel mesa-libGLU-devel glfw-devel

   # Arch Linux
   sudo pacman -S mesa glu glfw-x11
   ```

4. **Audio Libraries** (Optional - OpenAL)
   ```bash
   # Ubuntu/Debian
   sudo apt-get install libopenal-dev

   # Fedora
   sudo dnf install openal-soft-devel

   # Arch Linux
   sudo pacman -S openal
   ```

5. **JPEG Library** (Optional)
   ```bash
   # Ubuntu/Debian
   sudo apt-get install libjpeg-dev

   # Fedora
   sudo dnf install libjpeg-turbo-devel

   # Arch Linux
   sudo pacman -S libjpeg-turbo
   ```

### Building

**Option 1: Automated Build Script (Recommended)**

```bash
# Clone repository
git clone https://github.com/yourusername/MuMain.git
cd MuMain

# Run build script
./build-unix.sh

# Or with options
./build-unix.sh --type Release --clean
```

**Option 2: Manual CMake Build**

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . -j$(nproc)
```

### Running

```bash
cd build/bin
./main

# With server parameters
./main /u192.168.0.20 /p55902
```

---

## Building on macOS

### Prerequisites

1. **Xcode Command Line Tools**
   ```bash
   xcode-select --install
   ```

2. **Homebrew** (if not already installed)
   ```bash
   /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
   ```

3. **CMake**
   ```bash
   brew install cmake
   ```

4. **.NET 9 SDK**
   ```bash
   # Download and install from:
   # https://dotnet.microsoft.com/download/dotnet/9.0
   # Or use Homebrew cask:
   brew install --cask dotnet-sdk
   ```

5. **Graphics Libraries**
   ```bash
   # GLFW (optional - for cross-platform windowing)
   brew install glfw

   # OpenGL is included with macOS
   ```

6. **Audio** (Optional)
   ```bash
   # OpenAL is included with macOS
   # No additional installation needed
   ```

7. **JPEG Library** (Optional)
   ```bash
   brew install jpeg-turbo
   ```

### Building

**Option 1: Automated Build Script (Recommended)**

```bash
# Clone repository
git clone https://github.com/yourusername/MuMain.git
cd MuMain

# Run build script
./build-unix.sh

# Or with options
./build-unix.sh --type Release --clean
```

**Option 2: Manual CMake Build**

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . -j$(sysctl -n hw.ncpu)
```

### Running

```bash
cd build/bin
./main

# With server parameters
./main /u192.168.0.20 /p55902
```

### macOS-Specific Notes

- **Apple Silicon (M1/M2/M3)**: The build system will automatically detect ARM64 architecture
- **Code Signing**: You may need to allow the application in System Preferences > Security & Privacy
- **OpenAL**: macOS includes OpenAL framework, no separate installation needed
- **Native AOT**: .NET Native AOT supports both x64 and ARM64 on macOS

---

## Build Configurations

### Debug Build
- Optimizations disabled
- Debug symbols included
- Runtime checks enabled
- Useful for development and debugging

### Release Build
- Optimizations enabled (speed)
- Debug symbols included
- No runtime checks
- Recommended for gameplay

---

## Project Structure

```
MuMain/
├── CMakeLists.txt              # Root CMake configuration
├── BUILD.md                    # This file
├── README.md                   # Project overview
├── ClientLibrary/              # C# networking library (.NET 9 Native AOT)
│   ├── CMakeLists.txt
│   └── MUnique.Client.Library.csproj
└── Source Main 5.2/            # C++ client code
    ├── CMakeLists.txt
    ├── source/                 # C++ source files
    ├── dependencies/           # Third-party libraries
    │   ├── include/           # Header files (GLEW, TurboJPEG, etc.)
    │   └── lib/               # Static libraries (.lib files)
    └── Main.sln               # Original Visual Studio solution (reference)
```

---

## Common Build Issues

### Issue: Boost not found

**Solution**: Ensure Boost is installed at `C:\Libraries\boost_1_75_0` or specify the path:

```cmd
cmake .. -DBOOST_ROOT="C:\path\to\boost_1_75_0"
```

### Issue: .NET SDK not found

**Solution**: Install .NET 9 SDK and ensure `dotnet` is in your PATH:

```cmd
dotnet --version
```

Should output `9.0.x` or later.

### Issue: Link errors related to Boost

**Solution**: Ensure Boost was built with the correct settings:
- Static libraries (`link=static`)
- Static runtime (`runtime-link=static`)
- Correct architecture (Win32 or x64)
- Multithreaded (`threading=multi`)

### Issue: Missing wzAudio.lib or turbojpeg-static.lib

**Solution**: These files are included in the `dependencies/lib` directory. If missing, check:
1. Git LFS is enabled (for large binary files)
2. Files exist in `Source Main 5.2/dependencies/lib/`

---

## Advanced Configuration

### Custom Output Directory

```cmd
cmake .. -DCMAKE_RUNTIME_OUTPUT_DIRECTORY="C:\MuClient\bin"
```

### Disable C# Library Build

```cmd
cmake .. -DBUILD_CLIENT_LIBRARY=OFF
```

### Disable Main Executable Build

```cmd
cmake .. -DBUILD_MAIN_EXECUTABLE=OFF
```

---

## Cross-Platform Development Status

### Phase 1: Windows Support ✅
- ✅ CMake build system
- ✅ C# Native AOT integration
- ✅ All Windows dependencies configured
- 🚧 Testing and validation needed

### Phase 2: Platform Abstractions ✅
- ✅ Platform abstraction layer created (`source/Platform/`)
  - ✅ Window management (`PlatformWindow.h`)
  - ✅ Input handling (`PlatformInput.h`)
  - ✅ File I/O (`PlatformFile.h`)
  - ✅ Audio (`PlatformAudio.h`)
  - ✅ Time/Timer (`PlatformTime.h`)
- ✅ GLFW support for cross-platform windowing
- ✅ OpenAL support for cross-platform audio
- 🚧 Implementation of platform abstraction layer needed
- 🚧 Adaptation of existing code to use abstractions

### Phase 3: Linux Support ✅ (Build System Ready)
- ✅ CMake configuration for Linux
- ✅ Dependency detection (GLFW, OpenAL, etc.)
- ✅ Build script (`build-unix.sh`)
- ✅ Documentation complete
- 🚧 Platform-specific code compilation
- 🔲 Testing and validation

### Phase 4: macOS Support ✅ (Build System Ready)
- ✅ CMake configuration for macOS
- ✅ Dependency detection (GLFW, OpenAL, etc.)
- ✅ Build script (`build-unix.sh`)
- ✅ Documentation complete
- ✅ Apple Silicon (ARM64) support
- 🚧 Platform-specific code compilation
- 🔲 Testing and validation
- 🔲 Code signing and notarization

### Next Steps

The build system is now ready for all three platforms! However, the existing C++ code still uses Windows-specific APIs. To complete the cross-platform support:

1. **Implement Platform Abstraction Layer**
   - Write implementations for each platform in `source/Platform/`
   - Windows: Use existing Windows APIs
   - Linux: Use X11/Wayland + OpenAL
   - macOS: Use Cocoa + OpenAL

2. **Adapt Existing Code**
   - Replace Windows-specific calls with platform abstractions
   - Priority files:
     - `source/Winmain.cpp` → Use `Platform::Window`
     - `source/Input.cpp` → Use `Platform::Input`
     - File I/O scattered throughout → Use `Platform::File`
     - Audio calls → Use `Platform::Audio`

3. **Test on Each Platform**
   - Build and run on Windows 11
   - Build and run on Linux (Ubuntu 22.04+)
   - Build and run on macOS (Ventura+)

### Platform Compatibility Matrix

| Feature | Windows | Linux | macOS | Notes |
|---------|---------|-------|-------|-------|
| **Build System** | ✅ | ✅ | ✅ | CMake 3.25+ |
| **C# Library** | ✅ | ✅ | ✅ | .NET 9 Native AOT |
| **Graphics (OpenGL)** | ✅ | ✅ | ✅ | Via GLEW or system |
| **Windowing** | ✅ (Native) | 🚧 (GLFW) | 🚧 (GLFW) | Abstraction ready |
| **Input** | ✅ (Native) | 🚧 (GLFW) | 🚧 (GLFW) | Abstraction ready |
| **Audio** | ✅ (DirectSound) | 🚧 (OpenAL) | 🚧 (OpenAL) | Abstraction ready |
| **File I/O** | ✅ (Native) | 🚧 | 🚧 | Abstraction ready |
| **Networking** | ✅ | ✅ | ✅ | Pure .NET |

---

## Contributing

If you encounter build issues or have improvements to the build system, please:

1. Check existing issues: https://github.com/yourusername/MuMain/issues
2. Create a new issue with:
   - Operating system and version
   - CMake version
   - Visual Studio version (Windows)
   - Complete error message/log
   - Steps to reproduce

---

## Additional Resources

- **OpenMU Server**: https://github.com/MUnique/OpenMU
- **Original Source**: https://github.com/LouisEmulator/Main5.2
- **CMake Documentation**: https://cmake.org/documentation/
- **.NET Native AOT**: https://learn.microsoft.com/en-us/dotnet/core/deploying/native-aot/

---

## License

See `LICENSE` file for details.
