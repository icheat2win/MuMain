// PlatformWindow_GLFW.cpp - GLFW implementation of window management
// This implementation works on Windows, Linux, and macOS

#ifdef USE_GLFW

#include "PlatformWindow.h"
#include <GLFW/glfw3.h>
#include <cstring>
#include <cstdio>

namespace Platform
{
    // Static callbacks and state
    static bool s_GLFWInitialized = false;
    static Window* s_MainWindow = nullptr;

    // GLFW error callback
    static void GLFWErrorCallback(int error, const char* description)
    {
        fprintf(stderr, "GLFW Error %d: %s\n", error, description);
    }

    // GLFW window resize callback (needs to match declaration in header)
    void GLFWWindowSizeCallback(void* windowPtr, int width, int height)
    {
        GLFWwindow* window = static_cast<GLFWwindow*>(windowPtr);
        Window* platformWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
        if (platformWindow)
        {
            platformWindow->m_Width = width;
            platformWindow->m_Height = height;
        }
    }

    // Internal GLFW callback wrapper
    static void GLFWWindowSizeCallbackInternal(GLFWwindow* window, int width, int height)
    {
        GLFWWindowSizeCallback(static_cast<void*>(window), width, height);
    }

    // Window implementation
    Window::Window()
        : m_Handle(nullptr)
        , m_Width(0)
        , m_Height(0)
        , m_Fullscreen(false)
        , m_VSync(true)
    {
    }

    Window::~Window()
    {
        Destroy();
    }

    bool Window::Create(const WindowDesc& desc)
    {
        if (m_Handle)
        {
            fprintf(stderr, "Window already created\n");
            return false;
        }

        // Initialize GLFW if not already done
        if (!WindowManager::Initialize())
        {
            return false;
        }

        // Configure GLFW window hints
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_RESIZABLE, desc.resizable ? GLFW_TRUE : GLFW_FALSE);

        // Store dimensions
        m_Width = desc.width;
        m_Height = desc.height;
        m_Fullscreen = desc.fullscreen;
        m_VSync = desc.vsync;

        // Create window
        GLFWmonitor* monitor = desc.fullscreen ? glfwGetPrimaryMonitor() : nullptr;
        m_Handle = glfwCreateWindow(m_Width, m_Height, desc.title, monitor, nullptr);

        if (!m_Handle)
        {
            fprintf(stderr, "Failed to create GLFW window\n");
            return false;
        }

        // Set user pointer for callbacks
        glfwSetWindowUserPointer(static_cast<GLFWwindow*>(m_Handle), this);

        // Set callbacks
        glfwSetWindowSizeCallback(static_cast<GLFWwindow*>(m_Handle), GLFWWindowSizeCallbackInternal);

        // Make context current
        MakeContextCurrent();

        // Set VSync
        SetVSync(m_VSync);

        printf("GLFW Window created: %dx%d (%s)\n", m_Width, m_Height,
               m_Fullscreen ? "Fullscreen" : "Windowed");

        return true;
    }

    void Window::Destroy()
    {
        if (m_Handle)
        {
            glfwDestroyWindow(static_cast<GLFWwindow*>(m_Handle));
            m_Handle = nullptr;
        }
    }

    bool Window::IsOpen() const
    {
        return m_Handle != nullptr && !glfwWindowShouldClose(static_cast<GLFWwindow*>(m_Handle));
    }

    bool Window::ShouldClose() const
    {
        return m_Handle && glfwWindowShouldClose(static_cast<GLFWwindow*>(m_Handle));
    }

    void Window::SetShouldClose(bool shouldClose)
    {
        if (m_Handle)
        {
            glfwSetWindowShouldClose(static_cast<GLFWwindow*>(m_Handle), shouldClose ? GLFW_TRUE : GLFW_FALSE);
        }
    }

    void Window::SetTitle(const char* title)
    {
        if (m_Handle)
        {
            glfwSetWindowTitle(static_cast<GLFWwindow*>(m_Handle), title);
        }
    }

    void Window::GetSize(int& width, int& height) const
    {
        if (m_Handle)
        {
            glfwGetWindowSize(static_cast<GLFWwindow*>(m_Handle), &width, &height);
        }
        else
        {
            width = m_Width;
            height = m_Height;
        }
    }

    void Window::SetSize(int width, int height)
    {
        if (m_Handle)
        {
            glfwSetWindowSize(static_cast<GLFWwindow*>(m_Handle), width, height);
            m_Width = width;
            m_Height = height;
        }
    }

    void Window::GetPosition(int& x, int& y) const
    {
        if (m_Handle)
        {
            glfwGetWindowPos(static_cast<GLFWwindow*>(m_Handle), &x, &y);
        }
        else
        {
            x = 0;
            y = 0;
        }
    }

    void Window::SetPosition(int x, int y)
    {
        if (m_Handle)
        {
            glfwSetWindowPos(static_cast<GLFWwindow*>(m_Handle), x, y);
        }
    }

    void Window::SetFullscreen(bool fullscreen)
    {
        if (!m_Handle || m_Fullscreen == fullscreen)
            return;

        m_Fullscreen = fullscreen;

        if (fullscreen)
        {
            // Switch to fullscreen
            GLFWmonitor* monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* mode = glfwGetVideoMode(monitor);
            glfwSetWindowMonitor(static_cast<GLFWwindow*>(m_Handle), monitor,
                                0, 0, mode->width, mode->height, mode->refreshRate);
            m_Width = mode->width;
            m_Height = mode->height;
        }
        else
        {
            // Switch to windowed
            glfwSetWindowMonitor(static_cast<GLFWwindow*>(m_Handle), nullptr,
                                100, 100, 1024, 768, GLFW_DONT_CARE);
            m_Width = 1024;
            m_Height = 768;
        }
    }

    void Window::SetVSync(bool vsync)
    {
        m_VSync = vsync;
        glfwSwapInterval(vsync ? 1 : 0);
    }

    void Window::ProcessEvents()
    {
        glfwPollEvents();
    }

    void Window::SwapBuffers()
    {
        if (m_Handle)
        {
            glfwSwapBuffers(static_cast<GLFWwindow*>(m_Handle));
        }
    }

    bool Window::CreateOpenGLContext()
    {
        // GLFW creates the OpenGL context automatically with the window
        // Just make it current
        MakeContextCurrent();
        return true;
    }

    void Window::DestroyOpenGLContext()
    {
        // GLFW destroys the context with the window
        // Nothing to do here
    }

    void Window::MakeContextCurrent()
    {
        if (m_Handle)
        {
            glfwMakeContextCurrent(static_cast<GLFWwindow*>(m_Handle));
        }
    }

    void Window::Show()
    {
        if (m_Handle)
        {
            glfwShowWindow(static_cast<GLFWwindow*>(m_Handle));
        }
    }

    void Window::Hide()
    {
        if (m_Handle)
        {
            glfwHideWindow(static_cast<GLFWwindow*>(m_Handle));
        }
    }

    bool Window::IsFocused() const
    {
        if (!m_Handle)
        {
            return false;
        }

        int focused = glfwGetWindowAttrib(static_cast<GLFWwindow*>(m_Handle), GLFW_FOCUSED);
        return focused == GLFW_TRUE;
    }

    // WindowManager implementation
    namespace WindowManager
    {
        bool Initialize()
        {
            if (s_GLFWInitialized)
                return true;

            // Set error callback before init
            glfwSetErrorCallback(GLFWErrorCallback);

            if (!glfwInit())
            {
                fprintf(stderr, "Failed to initialize GLFW\n");
                return false;
            }

            s_GLFWInitialized = true;
            printf("GLFW initialized successfully\n");
            return true;
        }

        void Shutdown()
        {
            if (s_MainWindow)
            {
                delete s_MainWindow;
                s_MainWindow = nullptr;
            }

            if (s_GLFWInitialized)
            {
                glfwTerminate();
                s_GLFWInitialized = false;
                printf("GLFW terminated\n");
            }
        }

        Window* GetMainWindow()
        {
            return s_MainWindow;
        }

        bool CreateMainWindow(const WindowDesc& desc)
        {
            if (s_MainWindow)
            {
                fprintf(stderr, "Main window already exists\n");
                return false;
            }

            s_MainWindow = new Window();
            if (!s_MainWindow->Create(desc))
            {
                delete s_MainWindow;
                s_MainWindow = nullptr;
                return false;
            }

            return true;
        }

        void DestroyMainWindow()
        {
            if (s_MainWindow)
            {
                delete s_MainWindow;
                s_MainWindow = nullptr;
            }
        }
    }
}

#endif // USE_GLFW
