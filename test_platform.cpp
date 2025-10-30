// test_platform.cpp - Simple test program for Platform abstraction layer
// Compile with: clang++ -std=c++17 test_platform.cpp -I"Source Main 5.2/source" -lglfw -framework OpenGL -framework Cocoa -framework IOKit -o test_platform

#define USE_GLFW 1
#define PLATFORM_MACOS 1
#define PLATFORM_WINDOWS 0
#define PLATFORM_LINUX 0
#define GL_SILENCE_DEPRECATION  // Silence OpenGL deprecation warnings on macOS

#include "Source Main 5.2/source/Platform/Platform.h"
#include "Source Main 5.2/source/Platform/Platform.cpp"
#include "Source Main 5.2/source/Platform/PlatformWindow_GLFW.cpp"

#include <OpenGL/gl.h>
#include <cmath>
#include <cstdio>

int main(int argc, char* argv[])
{
    printf("=== Platform Abstraction Test ===\n\n");

    // Initialize platform
    if (!Platform::Initialize())
    {
        fprintf(stderr, "Failed to initialize platform\n");
        return 1;
    }

    // Create window
    Platform::WindowDesc windowDesc;
    windowDesc.title = "Platform Test - Press ESC to exit";
    windowDesc.width = 800;
    windowDesc.height = 600;
    windowDesc.fullscreen = false;
    windowDesc.resizable = true;
    windowDesc.vsync = true;

    if (!Platform::WindowManager::CreateMainWindow(windowDesc))
    {
        fprintf(stderr, "Failed to create window\n");
        Platform::Shutdown();
        return 1;
    }

    Platform::Window* window = Platform::WindowManager::GetMainWindow();
    printf("Window created successfully!\n");

    // Simple color animation
    float hue = 0.0f;
    int frameCount = 0;

    // Main loop
    while (window->IsOpen())
    {
        // Process events
        window->ProcessEvents();

        // Simple color animation (HSV to RGB)
        float r = (1.0f + sin(hue)) * 0.5f;
        float g = (1.0f + sin(hue + 2.0f)) * 0.5f;
        float b = (1.0f + sin(hue + 4.0f)) * 0.5f;

        // Clear screen with animated color
        glClearColor(r, g, b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw a simple triangle
        glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 0.0f, 0.0f); glVertex2f(0.0f, 0.5f);
        glColor3f(0.0f, 1.0f, 0.0f); glVertex2f(-0.5f, -0.5f);
        glColor3f(0.0f, 0.0f, 1.0f); glVertex2f(0.5f, -0.5f);
        glEnd();

        // Swap buffers
        window->SwapBuffers();

        // Update animation
        hue += 0.02f;
        frameCount++;

        if (frameCount % 60 == 0)
        {
            printf("Frame %d - Window is running...\n", frameCount);
        }
    }

    printf("\nTest completed successfully!\n");

    // Cleanup
    Platform::Shutdown();

    return 0;
}
