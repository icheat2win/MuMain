// Platform.h - Cross-platform abstraction layer
// This header provides platform detection and function declarations only
// Platform-specific includes are in Platform.cpp to avoid conflicts

#pragma once

// Platform detection
#if defined(_WIN32) || defined(_WIN64)
    #define PLATFORM_WINDOWS 1
    #define PLATFORM_LINUX 0
    #define PLATFORM_MACOS 0
#elif defined(__linux__)
    #define PLATFORM_WINDOWS 0
    #define PLATFORM_LINUX 1
    #define PLATFORM_MACOS 0
#elif defined(__APPLE__)
    #define PLATFORM_WINDOWS 0
    #define PLATFORM_LINUX 0
    #define PLATFORM_MACOS 1
#else
    #error "Unsupported platform"
#endif

// Forward declare platform window types
// Include PlatformWindow.h in your .cpp files if you need the full interface
namespace Platform
{
    class Window;
    struct WindowDesc;

    namespace WindowManager
    {
        bool Initialize();
        void Shutdown();
        Window* GetMainWindow();
        bool CreateMainWindow(const WindowDesc& desc);
        void DestroyMainWindow();
    }
}

// Common types across platforms
namespace Platform
{
    // Result codes
    enum class Result
    {
        Success = 0,
        Error = 1,
        NotSupported = 2,
        NotFound = 3
    };

    // Initialize platform layer
    bool Initialize();

    // Shutdown platform layer
    void Shutdown();

    // Get platform name
    const char* GetPlatformName();

    // Get executable directory
    const char* GetExecutableDirectory();
}
