// Platform.cpp - Cross-platform initialization and utilities

#include "Platform.h"
#include "PlatformWindow.h"
#include "PlatformInput.h"
#include <cstdio>
#include <cstring>

#if PLATFORM_WINDOWS
    #include <windows.h>
#elif PLATFORM_LINUX
    #include <unistd.h>
    #include <limits.h>
#elif PLATFORM_MACOS
    #include <mach-o/dyld.h>
    #include <limits.h>
#endif

namespace Platform
{
    // Static storage for executable directory
    static char s_ExecutableDirectory[512] = {0};

    bool Initialize()
    {
        printf("Initializing Platform Layer...\n");

        // Get executable directory
        #if PLATFORM_WINDOWS
            char exePath[512];
            GetModuleFileNameA(NULL, exePath, sizeof(exePath));

            // Find last backslash
            char* lastSlash = strrchr(exePath, '\\');
            if (lastSlash)
            {
                *lastSlash = '\0';
                strcpy(s_ExecutableDirectory, exePath);
            }
        #elif PLATFORM_LINUX
            char exePath[PATH_MAX];
            ssize_t len = readlink("/proc/self/exe", exePath, sizeof(exePath) - 1);
            if (len != -1)
            {
                exePath[len] = '\0';
                char* lastSlash = strrchr(exePath, '/');
                if (lastSlash)
                {
                    *lastSlash = '\0';
                    strcpy(s_ExecutableDirectory, exePath);
                }
            }
        #elif PLATFORM_MACOS
            char exePath[PATH_MAX];
            uint32_t size = sizeof(exePath);
            if (_NSGetExecutablePath(exePath, &size) == 0)
            {
                char* lastSlash = strrchr(exePath, '/');
                if (lastSlash)
                {
                    *lastSlash = '\0';
                    strcpy(s_ExecutableDirectory, exePath);
                }
            }
        #endif

        printf("Platform: %s\n", GetPlatformName());
        printf("Executable Directory: %s\n", s_ExecutableDirectory);

        if (!Input::Initialize())
        {
            fprintf(stderr, "Failed to initialize input system\n");
            return false;
        }

        // Initialize window manager
        if (!WindowManager::Initialize())
        {
            fprintf(stderr, "Failed to initialize WindowManager\n");
            return false;
        }

        printf("Platform Layer initialized successfully\n");
        return true;
    }

    void Shutdown()
    {
        printf("Shutting down Platform Layer...\n");

        // Shutdown input and window manager
        Input::Shutdown();
        WindowManager::Shutdown();

        printf("Platform Layer shut down\n");
    }

    const char* GetPlatformName()
    {
        #if PLATFORM_WINDOWS
            return "Windows";
        #elif PLATFORM_LINUX
            return "Linux";
        #elif PLATFORM_MACOS
            return "macOS";
        #else
            return "Unknown";
        #endif
    }

    const char* GetExecutableDirectory()
    {
        return s_ExecutableDirectory;
    }
}
