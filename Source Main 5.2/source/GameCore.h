// GameCore.h - Platform-independent game interface
// This header provides the interface for initializing and running the MU Online game
// It can be called from both Winmain.cpp (Windows) and main_crossplatform.cpp (cross-platform)

#pragma once

#include <cstdint>

// Forward declarations (avoid including platform-specific headers)
#ifdef _WIN32
    typedef struct HDC__* HDC;
    typedef struct HWND__* HWND;
#else
    typedef void* HDC;
    typedef void* HWND;
#endif

namespace GameCore
{
    // Initialization configuration
    struct Config
    {
        int windowWidth = 1024;
        int windowHeight = 768;
        bool windowed = true;
        bool soundEnabled = true;
        bool musicEnabled = true;
        int colorDepth = 32;

        // Network connection
        const char* serverIP = nullptr;
        uint16_t serverPort = 0;
        bool connectMode = false;

        // Language
        const wchar_t* language = L"Eng";

        // Saved login info (if RememberMe was checked)
        wchar_t savedID[256] = {0};
        wchar_t savedPassword[256] = {0};
        bool rememberMe = false;
    };

    // Initialization result
    enum class InitResult
    {
        Success = 0,
        ErrorOpenGL = 1,
        ErrorAudio = 2,
        ErrorDataLoad = 3,
        ErrorMemory = 4,
        ErrorOther = 5
    };

    // ================================================================
    // INITIALIZATION FUNCTIONS
    // ================================================================

    // Phase 1: Early initialization (before window/OpenGL)
    // - Load configuration from registry/file
    // - Initialize error reporting
    // - Parse command line arguments
    InitResult PreInitialize(Config& config, int argc, char** argv);

    // Phase 2: Post-window initialization (after OpenGL context is ready)
    // - Initialize fonts (on Windows) or font rendering system
    // - Initialize input system
    // - Initialize UI system
    // - Initialize audio
    // - Load game data
    // - Initialize game systems
    InitResult PostInitialize(const Config& config, HWND hWnd, HDC hDC);

    // ================================================================
    // GAME LOOP FUNCTIONS
    // ================================================================

    // Update game logic for one frame
    // deltaTime: Time elapsed since last frame in seconds
    void Update(float deltaTime);

    // Render the game scene
    // hDC: Device context (can be null for cross-platform builds)
    void Render(HDC hDC);

    // Process input events
    // On Windows: Called after Windows message processing
    // On cross-platform: Called with GLFW input state
    void ProcessInput();

    // Check if the game wants to quit
    bool ShouldQuit();

    // ================================================================
    // SHUTDOWN FUNCTIONS
    // ================================================================

    // Cleanup and shutdown
    void Shutdown();

    // ================================================================
    // UTILITY FUNCTIONS
    // ================================================================

    // Get current window dimensions (useful for resize events)
    void SetWindowSize(int width, int height);

    // Window focus changed
    void SetWindowActive(bool active);

    // VSync control
    void EnableVSync(bool enable);

    // Frame rate control
    void SetTargetFPS(int fps); // -1 for unlimited
    bool CheckRenderNextFrame();
}

// ================================================================
// IMPLEMENTATION NOTES
// ================================================================
//
// For Windows builds (Winmain.cpp):
// - PostInitialize() will use Windows-specific code (fonts, DirectSound, etc.)
// - Render() will receive HDC and use SwapBuffers directly
//
// For cross-platform builds (main_crossplatform.cpp):
// - PostInitialize() will use GLFW/OpenAL alternatives
// - Render() will receive null HDC and use GLFW's swap buffers
// - Some features may be stubbed initially (fonts, audio, etc.)
//
// Migration strategy:
// 1. Start by wrapping existing Winmain.cpp code in GameCore functions
// 2. Gradually refactor each subsystem to be platform-independent
// 3. Use conditional compilation (#ifdef PLATFORM_WINDOWS) where needed
// 4. Eventually, most code should work on all platforms
//
