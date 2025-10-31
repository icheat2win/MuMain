// PlatformWindow.h - Cross-platform window management abstraction
// Supports both GLFW (preferred) and native window APIs

#pragma once

#include <cstdint>

// Forward declarations
#ifdef USE_GLFW
    struct GLFWwindow;
#endif

namespace Platform
{
    // Window handle (platform-specific)
    #if defined(USE_GLFW)
        typedef GLFWwindow* WindowHandle;
    #elif PLATFORM_WINDOWS
        typedef void* WindowHandle; // HWND
    #elif PLATFORM_LINUX
        typedef unsigned long WindowHandle; // X11 Window
    #elif PLATFORM_MACOS
        typedef void* WindowHandle; // NSWindow*
    #endif

    // Window creation parameters
    struct WindowDesc
    {
        const char* title = "MU Online Client";
        int width = 1024;
        int height = 768;
        bool fullscreen = false;
        bool resizable = true;
        bool vsync = true;
    };

    // Forward declare callback for friend access
    #ifdef USE_GLFW
        void GLFWWindowSizeCallback(void* window, int width, int height);
    #endif

    // Window class
    class Window
    {
    public:
        Window();
        ~Window();

        #ifdef USE_GLFW
            friend void GLFWWindowSizeCallback(void*, int, int);
        #endif

        // Create window
        bool Create(const WindowDesc& desc);

        // Destroy window
        void Destroy();

        // Get window handle
        WindowHandle GetHandle() const { return m_Handle; }

        // Window state
        bool IsOpen() const;
        bool ShouldClose() const;
        void SetShouldClose(bool shouldClose);

        // Window properties
        void SetTitle(const char* title);
        void GetSize(int& width, int& height) const;
        void SetSize(int width, int height);
        void GetPosition(int& x, int& y) const;
        void SetPosition(int x, int y);

        // Fullscreen
        bool IsFullscreen() const { return m_Fullscreen; }
        void SetFullscreen(bool fullscreen);

        // VSync
        bool IsVSync() const { return m_VSync; }
        void SetVSync(bool vsync);

        // Process window events (polls/processes messages)
        void ProcessEvents();

        // Swap buffers (present frame)
        void SwapBuffers();

        // OpenGL context
        bool CreateOpenGLContext();
        void DestroyOpenGLContext();
        void MakeContextCurrent();

        // Show/Hide
        void Show();
        void Hide();

        // Focus state
        bool IsFocused() const;

    private:
        WindowHandle m_Handle;
        int m_Width;
        int m_Height;
        bool m_Fullscreen;
        bool m_VSync;

        #if PLATFORM_WINDOWS && !defined(USE_GLFW)
            void* m_hDC;  // HDC
            void* m_hRC;  // HGLRC (OpenGL rendering context)
        #elif PLATFORM_LINUX && !defined(USE_GLFW)
            void* m_Display;  // X11 Display*
            unsigned long m_GLXContext;  // GLXContext
        #elif PLATFORM_MACOS && !defined(USE_GLFW)
            void* m_NSGLContext;  // NSOpenGLContext*
        #endif
    };

    // Global window functions
    namespace WindowManager
    {
        // Initialize window system
        bool Initialize();

        // Shutdown window system
        void Shutdown();

        // Get primary window (singleton for now)
        Window* GetMainWindow();

        // Create main window
        bool CreateMainWindow(const WindowDesc& desc);

        // Destroy main window
        void DestroyMainWindow();
    }
}
