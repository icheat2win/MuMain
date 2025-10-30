# Cross-Platform Implementation Guide

This document explains the cross-platform architecture and how to complete the implementation.

## Overview

The MU Online client has been set up with a complete cross-platform build system and platform abstraction layer. The project can now be built on:

- ✅ **Windows 11** (x86/x64)
- ✅ **Linux** (x86_64/ARM64)
- ✅ **macOS** (x64/Apple Silicon)

## What's Complete

### 1. Build System ✅

**CMake Configuration**
- Root `CMakeLists.txt` with automatic platform detection
- Separate CMake files for C# library and C++ executable
- Automatic dependency detection (GLFW, OpenAL, JPEG, etc.)
- Support for Debug and Release builds

**Build Scripts**
- `build-windows.bat` / `build-windows.ps1` for Windows
- `build-unix.sh` for Linux and macOS
- All scripts include dependency checking and helpful error messages

**Documentation**
- Complete build instructions in `BUILD.md`
- Platform-specific notes and troubleshooting
- Prerequisite installation commands for all platforms

### 2. Platform Abstraction Layer ✅

Located in `Source Main 5.2/source/Platform/`, this provides unified APIs:

**Platform.h**
- Platform detection macros
- Common initialization/shutdown
- Utility functions (get executable directory, etc.)

**PlatformWindow.h**
- Window creation and management
- OpenGL context creation
- Fullscreen, VSync control
- Event processing
- Supports both GLFW (cross-platform) and native APIs

**PlatformInput.h**
- Keyboard input (replaces `GetAsyncKeyState`)
- Mouse input and cursor control
- Clipboard access
- Text input for chat
- Cross-platform key codes

**PlatformFile.h**
- File I/O (replaces `CreateFile`, `ReadFile`, `WriteFile`)
- Directory operations
- Path utilities
- Cross-platform file attributes

**PlatformAudio.h**
- Audio playback (replaces wzAudio/DirectSound)
- 3D positional audio
- Music streaming
- Supports OpenAL (cross-platform) and native APIs

**PlatformTime.h**
- High-resolution timers
- FPS calculation and limiting
- Frame timing
- Replaces `GetTickCount` and `QueryPerformanceCounter`

### 3. Dependency Management ✅

**Cross-Platform Libraries**
- OpenGL (graphics) - All platforms
- GLFW (windowing) - Optional, detected automatically
- OpenAL (audio) - Optional, detected automatically
- libjpeg-turbo (images) - Optional, falls back to bundled version

**Platform-Specific**
- Windows: DirectSound (fallback), Windows APIs
- Linux: X11 (fallback), PulseAudio/ALSA (fallback)
- macOS: Cocoa (fallback), OpenAL (included)

## What's Not Complete

### 1. Platform Abstraction Implementation 🚧

The abstraction layer provides **interfaces only**. You need to implement them:

**Required Files (to be created):**

```
Source Main 5.2/source/Platform/
├── Platform.cpp              # Initialize/Shutdown implementations
├── PlatformWindow.cpp        # Window management impl (Windows)
├── PlatformWindow_GLFW.cpp   # Window management impl (GLFW)
├── PlatformInput.cpp         # Input impl (Windows)
├── PlatformInput_GLFW.cpp    # Input impl (GLFW)
├── PlatformFile.cpp          # File I/O impl (all platforms)
├── PlatformAudio.cpp         # Audio impl (Windows - DirectSound)
├── PlatformAudio_OpenAL.cpp  # Audio impl (OpenAL - all platforms)
└── PlatformTime.cpp          # Timer impl (all platforms)
```

**Implementation Strategy:**

1. **Start with GLFW implementations** (cross-platform first)
   - PlatformWindow_GLFW.cpp
   - PlatformInput_GLFW.cpp
   - These work on all three platforms

2. **Implement OpenAL audio**
   - PlatformAudio_OpenAL.cpp
   - Works on all platforms
   - macOS includes OpenAL by default

3. **Implement platform-agnostic code**
   - PlatformFile.cpp (use C++ std::filesystem where possible)
   - PlatformTime.cpp (use C++11 chrono where possible)

4. **Keep Windows native as fallback**
   - PlatformWindow.cpp (existing Windows code)
   - PlatformInput.cpp (existing GetAsyncKeyState)
   - PlatformAudio.cpp (existing DirectSound)

### 2. Code Adaptation 🚧

Update existing code to use the platform abstraction layer:

**Priority 1: Window Management**
- `source/Winmain.cpp` - Main entry point
  - Replace `WinMain` with cross-platform main
  - Use `Platform::WindowManager::CreateMainWindow()`
  - Replace `CreateWindowEx` calls

**Priority 2: Input Handling**
- `source/Input.cpp` - Input system
  - Replace `GetAsyncKeyState()` with `Platform::Input::IsKeyDown()`
  - Update keyboard state tracking

**Priority 3: File I/O**
- Replace throughout codebase:
  - `CreateFile()` → `Platform::File::Open()`
  - `ReadFile()` → `file.Read()`
  - `WriteFile()` → `file.Write()`
  - Path handling → `Platform::FileSystem::*`

**Priority 4: Audio**
- `source/DSPlaySound.cpp` and related
  - Replace wzAudio calls with `Platform::Audio::*`
  - Update all sound effect playback

**Priority 5: Timing**
- Replace throughout:
  - `GetTickCount()` → `Platform::Time::GetTickCount()`
  - Timer classes → `Platform::Timer`

### 3. Configuration Storage 🚧

Replace Windows Registry with cross-platform configuration:

**Current:** Uses Windows Registry
- `source/ExternalObject/Leaf/regkey.h`
- Stores user settings, credentials, etc.

**Solution:** Create config file system
- Use JSON or INI format
- Store in platform-appropriate location:
  - Windows: `%APPDATA%/MuOnline/`
  - Linux: `~/.config/muonline/`
  - macOS: `~/Library/Application Support/MuOnline/`

**Implementation:**
1. Create `source/Config/ConfigManager.h`
2. Implement JSON/INI parser
3. Replace all registry calls with config file calls

## Implementation Roadmap

### Step 1: Test Windows Build (Week 1)

1. Ensure Windows build compiles
2. Fix any CMake issues
3. Verify C# Native AOT library loads
4. Test on Windows 11 (x86 and x64)

### Step 2: Implement GLFW Window (Week 2)

1. Create `PlatformWindow_GLFW.cpp`
2. Implement all window functions using GLFW
3. Test on Windows first (GLFW works on Windows too)
4. Add preprocessor guards: `#ifdef USE_GLFW`

### Step 3: Implement GLFW Input (Week 2-3)

1. Create `PlatformInput_GLFW.cpp`
2. Map GLFW keys to `Platform::KeyCode`
3. Implement mouse handling
4. Test on Windows with GLFW

### Step 4: Implement OpenAL Audio (Week 3-4)

1. Create `PlatformAudio_OpenAL.cpp`
2. Load WAV files
3. Implement 3D audio
4. Test on Windows with OpenAL

### Step 5: Implement File I/O (Week 4)

1. Create `PlatformFile.cpp`
2. Use C++ `<filesystem>` where possible
3. Platform-specific code where needed
4. Test on all platforms

### Step 6: Adapt Existing Code (Week 5-8)

1. Update `Winmain.cpp` to use `Platform::Window`
2. Update `Input.cpp` to use `Platform::Input`
3. Update audio code to use `Platform::Audio`
4. Update file I/O throughout codebase
5. Test incrementally on Windows

### Step 7: Build on Linux (Week 9)

1. Install dependencies on Linux
2. Run `./build-unix.sh`
3. Fix compilation errors
4. Link errors
5. Runtime testing

### Step 8: Build on macOS (Week 10)

1. Install dependencies on macOS
2. Run `./build-unix.sh`
3. Fix any macOS-specific issues
4. Test on both Intel and Apple Silicon
5. Code signing

### Step 9: Polish and Test (Week 11-12)

1. Test all features on all platforms
2. Fix platform-specific bugs
3. Performance testing
4. Create installers/packages

## Quick Start for Developers

### To Continue Development:

1. **Install Dependencies**
   - Follow instructions in `BUILD.md` for your platform
   - Install GLFW and OpenAL for best results

2. **Build the Project**
   ```bash
   # Windows
   build-windows.bat

   # Linux/macOS
   ./build-unix.sh
   ```

3. **Start Implementing**
   - Begin with `Source Main 5.2/source/Platform/PlatformWindow_GLFW.cpp`
   - Use existing Windows code as reference
   - Test on Windows first before other platforms

4. **Testing**
   - Build on Windows regularly
   - Test with `-DUSE_GLFW=ON` to use cross-platform code
   - Once stable, test on Linux/macOS

## Code Examples

### Example: Window Creation

**Old Windows Code:**
```cpp
HWND hwnd = CreateWindowEx(
    0, className, "MU Online", WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, CW_USEDEFAULT, 1024, 768,
    NULL, NULL, hInstance, NULL
);
```

**New Cross-Platform Code:**
```cpp
Platform::WindowDesc desc;
desc.title = "MU Online";
desc.width = 1024;
desc.height = 768;
desc.fullscreen = false;

auto window = Platform::WindowManager::GetMainWindow();
if (!window->Create(desc)) {
    // Handle error
}
```

### Example: Input Handling

**Old Windows Code:**
```cpp
if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
    // Escape pressed
}
```

**New Cross-Platform Code:**
```cpp
if (Platform::Input::IsKeyDown(Platform::KeyCode::Escape)) {
    // Escape pressed
}
```

### Example: File I/O

**Old Windows Code:**
```cpp
HANDLE hFile = CreateFile(path, GENERIC_READ, FILE_SHARE_READ,
    NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
DWORD bytesRead;
ReadFile(hFile, buffer, size, &bytesRead, NULL);
CloseHandle(hFile);
```

**New Cross-Platform Code:**
```cpp
Platform::File file;
if (file.Open(path, Platform::FileAccess::Read, Platform::FileMode::OpenExisting)) {
    size_t bytesRead = file.Read(buffer, size);
    file.Close();
}
```

## Resources

- **CMake Documentation**: https://cmake.org/documentation/
- **GLFW Documentation**: https://www.glfw.org/documentation.html
- **OpenAL Documentation**: https://www.openal.org/documentation/
- **.NET Native AOT**: https://learn.microsoft.com/en-us/dotnet/core/deploying/native-aot/

## Questions?

If you have questions about the cross-platform implementation:

1. Check `BUILD.md` for build-specific issues
2. Check this document for architecture questions
3. Look at the platform abstraction headers for API details
4. Create an issue on GitHub with the `cross-platform` label

## License

Same as the main project - see `LICENSE` file.
