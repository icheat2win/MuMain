// GameCore.cpp - Platform-independent game implementation
// This file provides the core game logic that works on all platforms

#include "GameCore.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <ctime>

// TODO: Include game headers as they become platform-independent
// For now, we'll use forward declarations and external variables

// ================================================================
// TEMPORARY: External game variables and functions
// These will be properly included once the headers are fixed
// ================================================================

// From Winmain.cpp - global variables we need
extern int WindowWidth;
extern int WindowHeight;
extern bool g_bWndActive;
extern float g_fScreenRate_x;
extern float g_fScreenRate_y;

// From ZzzScene.cpp - the main rendering function
extern void RenderScene(HDC hDC);

// TODO: Declare other external functions as needed:
// - InitDirectSound
// - wzAudioCreate
// - CInput::Instance().Create()
// - etc.

namespace GameCore
{
    // ================================================================
    // INTERNAL STATE
    // ================================================================

    static Config s_Config;
    static bool s_Initialized = false;
    static bool s_ShouldQuit = false;
    static int s_TargetFPS = -1;
    static float s_LastFrameTime = 0.0f;

    // ================================================================
    // PHASE 1: PRE-INITIALIZATION
    // ================================================================

    InitResult PreInitialize(Config& config, int argc, char** argv)
    {
        printf("[GameCore] Pre-initialization started\n");

        // Parse command line for server connection
        for (int i = 1; i < argc; i++)
        {
            const char* arg = argv[i];

            // /uIP format
            if (arg[0] == '/' && (arg[1] == 'u' || arg[1] == 'U'))
            {
                config.serverIP = arg + 2;
                config.connectMode = true;
                printf("[GameCore] Server IP: %s\n", config.serverIP);
            }
            // /pPORT format
            else if (arg[0] == '/' && (arg[1] == 'p' || arg[1] == 'P'))
            {
                config.serverPort = static_cast<uint16_t>(atoi(arg + 2));
                printf("[GameCore] Server Port: %d\n", config.serverPort);
            }
            // connect command
            else if (strcmp(arg, "connect") == 0)
            {
                config.connectMode = true;
            }
        }

        // Initialize random seed
        srand(static_cast<unsigned>(time(nullptr)));

        printf("[GameCore] Pre-initialization complete\n");
        return InitResult::Success;
    }

    // ================================================================
    // PHASE 2: POST-INITIALIZATION
    // ================================================================

    InitResult PostInitialize(const Config& config, HWND hWnd, HDC hDC)
    {
        printf("[GameCore] Post-initialization started\n");

        // Save configuration
        s_Config = config;

        // Set global window dimensions
        WindowWidth = config.windowWidth;
        WindowHeight = config.windowHeight;
        g_fScreenRate_x = static_cast<float>(WindowWidth) / 640.0f;
        g_fScreenRate_y = static_cast<float>(WindowHeight) / 480.0f;

        printf("[GameCore] Window size: %dx%d\n", WindowWidth, WindowHeight);
        printf("[GameCore] Screen rate: %.2fx%.2f\n", g_fScreenRate_x, g_fScreenRate_y);

        // TODO: Initialize game systems
        // This is where we'll gradually add the initialization code from Winmain.cpp:
        //
        // 1. Fonts (Windows) or font rendering (cross-platform)
        // 2. Input system: CInput::Instance().Create()
        // 3. UI system: g_pNewUISystem->Create()
        // 4. Audio: wzAudioCreate() or OpenAL
        // 5. Sound: InitDirectSound() or OpenAL
        // 6. Game data: GateAttribute, ItemAttribute, etc.
        // 7. Character system: CharacterMachine->Init()
        // 8. UI managers: g_pUIManager, g_pUIMapName
        // 9. Game systems: BuffSystem, MapProcess, PetProcess
        // 10. Input boxes: g_pMercenaryInputBox, etc.

        s_Initialized = true;
        printf("[GameCore] Post-initialization complete\n");
        return InitResult::Success;
    }

    // ================================================================
    // GAME LOOP FUNCTIONS
    // ================================================================

    void Update(float deltaTime)
    {
        if (!s_Initialized)
            return;

        // TODO: Call game update logic
        // This will include:
        // - Network packet processing
        // - Game state updates
        // - Character movement
        // - Animation updates
        // - Physics
        // - AI
        // etc.

        s_LastFrameTime = deltaTime;
    }

    void Render(HDC hDC)
    {
        if (!s_Initialized)
            return;

        // Call the main rendering function from ZzzScene.cpp
        // This function already exists and does all the OpenGL rendering
        RenderScene(hDC);
    }

    void ProcessInput()
    {
        if (!s_Initialized)
            return;

        // TODO: Process input
        // On Windows: input is already processed by message loop
        // On cross-platform: need to query GLFW input state
    }

    bool ShouldQuit()
    {
        return s_ShouldQuit;
    }

    // ================================================================
    // SHUTDOWN FUNCTIONS
    // ================================================================

    void Shutdown()
    {
        printf("[GameCore] Shutting down\n");

        // TODO: Cleanup game systems in reverse order of initialization:
        // 1. Delete game systems (PetProcess, MapProcess, BuffSystem)
        // 2. Delete UI managers
        // 3. Delete CharacterMachine
        // 4. Delete game data arrays
        // 5. Shutdown audio
        // 6. Shutdown UI system
        // 7. Shutdown input system
        // 8. Delete fonts

        s_Initialized = false;
        printf("[GameCore] Shutdown complete\n");
    }

    // ================================================================
    // UTILITY FUNCTIONS
    // ================================================================

    void SetWindowSize(int width, int height)
    {
        WindowWidth = width;
        WindowHeight = height;
        g_fScreenRate_x = static_cast<float>(WindowWidth) / 640.0f;
        g_fScreenRate_y = static_cast<float>(WindowHeight) / 480.0f;

        printf("[GameCore] Window resized: %dx%d\n", width, height);
    }

    void SetWindowActive(bool active)
    {
        g_bWndActive = active;
    }

    void EnableVSync(bool enable)
    {
        // TODO: Platform-specific VSync control
        // On Windows: wglSwapIntervalEXT
        // On GLFW: glfwSwapInterval
        printf("[GameCore] VSync %s\n", enable ? "enabled" : "disabled");
    }

    void SetTargetFPS(int fps)
    {
        s_TargetFPS = fps;
        printf("[GameCore] Target FPS: %d\n", fps < 0 ? -1 : fps);
    }

    bool CheckRenderNextFrame()
    {
        // TODO: Implement frame rate limiting
        // For now, always render
        return true;
    }
}
