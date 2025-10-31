// main_crossplatform.cpp - Cross-platform entry point for MU Online Client
// This file provides a cross-platform main() that uses the Platform abstraction layer
// It can coexist with Winmain.cpp during the migration process

// Only compile this when using cross-platform mode
#ifdef USE_CROSSPLATFORM_MAIN

// Include standard headers first to avoid conflicts
#include <cstdio>
#include <cstring>
#include <cstdlib>  // for atoi

// Define OpenGL includes before Platform.h to avoid conflicts
#ifdef __APPLE__
    #define GL_SILENCE_DEPRECATION
    #include <OpenGL/gl.h>
#elif defined(_WIN32)
    #include <GL/gl.h>
#else
    #include <GL/gl.h>
#endif

// Now include Platform headers
#include "Platform/Platform.h"
#include "Platform/PlatformWindow.h"

// Include game core interface
#include "GameCore.h"

// Command line parsing is now handled by GameCore::PreInitialize()

// Cross-platform main entry point
int main(int argc, char* argv[])
{
    printf("=================================================\n");
    printf("    MU Online Client - Cross-Platform Build\n");
    printf("=================================================\n\n");

    // ================================================================
    // PHASE 1: PRE-INITIALIZATION
    // ================================================================

    // Initialize game configuration and parse command line
    GameCore::Config gameConfig;
    gameConfig.windowWidth = 1024;
    gameConfig.windowHeight = 768;
    gameConfig.windowed = true;
    gameConfig.soundEnabled = true;
    gameConfig.musicEnabled = true;

    GameCore::InitResult initResult = GameCore::PreInitialize(gameConfig, argc, argv);
    if (initResult != GameCore::InitResult::Success)
    {
        fprintf(stderr, "Failed to pre-initialize game\n");
        return 1;
    }

    printf("Platform: %s\n", Platform::GetPlatformName());
    if (gameConfig.connectMode)
    {
        printf("Connect Mode: %s:%d\n", gameConfig.serverIP, gameConfig.serverPort);
    }
    printf("\n");

    // ================================================================
    // PHASE 2: PLATFORM INITIALIZATION
    // ================================================================

    // Initialize platform layer
    printf("Initializing Platform Layer...\n");
    if (!Platform::Initialize())
    {
        fprintf(stderr, "Failed to initialize platform layer\n");
        return 1;
    }

    // Create window
    printf("Creating game window...\n");
    Platform::WindowDesc windowDesc;
    windowDesc.title = "MU Online Client";
    windowDesc.width = gameConfig.windowWidth;
    windowDesc.height = gameConfig.windowHeight;
    windowDesc.fullscreen = !gameConfig.windowed;
    windowDesc.resizable = true;
    windowDesc.vsync = true;

    if (!Platform::WindowManager::CreateMainWindow(windowDesc))
    {
        fprintf(stderr, "Failed to create window\n");
        Platform::Shutdown();
        return 1;
    }

    Platform::Window* window = Platform::WindowManager::GetMainWindow();
    printf("Window created successfully!\n\n");

    // ================================================================
    // PHASE 3: GAME INITIALIZATION
    // ================================================================

    // Initialize game systems (after OpenGL context is ready)
    printf("Initializing game...\n");
    initResult = GameCore::PostInitialize(gameConfig, nullptr, nullptr);
    if (initResult != GameCore::InitResult::Success)
    {
        fprintf(stderr, "Failed to initialize game (error %d)\n", static_cast<int>(initResult));
        Platform::Shutdown();
        return 1;
    }
    printf("Game initialized!\n\n");

    printf("=================================================\n");
    printf("    Game Running - Press ESC to exit\n");
    printf("=================================================\n\n");

    // ================================================================
    // MAIN GAME LOOP
    // ================================================================

    int frameCount = 0;
    float lastTime = 0.0f; // TODO: Use Platform::Time when implemented

    while (window->IsOpen() && !GameCore::ShouldQuit())
    {
        // Process window events and update input state
        window->ProcessEvents();
        Platform::Input::Update();

        // TODO: Get real delta time from Platform::Time
        float currentTime = frameCount * 0.016f; // Fake 60 FPS for now
        float deltaTime = 0.016f;

        // Process input
        GameCore::ProcessInput();

        // Update game logic
        GameCore::Update(deltaTime);

        // Render game
        GameCore::Render(nullptr); // nullptr for HDC on cross-platform

        // Swap buffers
        window->SwapBuffers();

        frameCount++;

        // Print status every 300 frames (5 seconds at 60 FPS)
        if (frameCount % 300 == 0)
        {
            printf("Frame %d - Game running...\n", frameCount);
        }
    }

    // ================================================================
    // SHUTDOWN
    // ================================================================

    printf("\n=================================================\n");
    printf("    Shutting Down\n");
    printf("=================================================\n\n");

    printf("Shutting down game...\n");
    GameCore::Shutdown();

    printf("Shutting down platform layer...\n");
    Platform::Shutdown();

    printf("\nMU Online Client terminated successfully.\n");
    return 0;
}

#endif // USE_CROSSPLATFORM_MAIN
