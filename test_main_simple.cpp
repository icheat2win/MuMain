// Simple test of main_crossplatform without the game source directory in include path
// This avoids the _types.h conflict

#include <cstdio>
#include <cstring>
#include <cstdlib>

// Define OpenGL includes
#ifdef __APPLE__
    #include <OpenGL/gl.h>
#elif defined(_WIN32)
    #include <GL/gl.h>
#else
    #include <GL/gl.h>
#endif

// Define platform macros
#define USE_GLFW 1
#define USE_CROSSPLATFORM_MAIN 1
#define PLATFORM_MACOS 1
#define PLATFORM_WINDOWS 0
#define PLATFORM_LINUX 0

// Forward declarations instead of including PlatformWindow.h
namespace Platform
{
    enum class Result { Success = 0, Error = 1, NotSupported = 2, NotFound = 3 };

    bool Initialize();
    void Shutdown();
    const char* GetPlatformName();
    const char* GetExecutableDirectory();

    struct WindowDesc
    {
        const char* title = "Window";
        int width = 1024;
        int height = 768;
        bool fullscreen = false;
        bool resizable = true;
        bool vsync = true;
    };

    class Window
    {
    public:
        bool IsOpen() const;
        void ProcessEvents();
        void SwapBuffers();
        int GetWidth() const { return m_Width; }
        int GetHeight() const { return m_Height; }

        // Allow direct access for Platform.cpp
        void* m_Handle;
        int m_Width;
        int m_Height;
        bool m_Fullscreen;
        bool m_VSync;
    };

    namespace WindowManager
    {
        Window* GetMainWindow();
        bool CreateMainWindow(const WindowDesc& desc);
    }
}

// Game stubs
namespace GameStubs
{
    void InitializeGame()
    {
        printf("Game initialization (stub)\n");
    }

    void UpdateGame(float deltaTime)
    {
        // Game update logic stub
    }

    void RenderGame()
    {
        glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void ShutdownGame()
    {
        printf("Game shutdown (stub)\n");
    }
}

// Connection parameters
struct ConnectionParams
{
    char serverIP[256] = "127.0.0.1";
    int serverPort = 44406;
    bool connectMode = false;
};

bool ParseCommandLine(int argc, char* argv[], ConnectionParams& params)
{
    for (int i = 1; i < argc; i++)
    {
        const char* arg = argv[i];

        if (arg[0] == '/' && (arg[1] == 'u' || arg[1] == 'U'))
        {
            strncpy(params.serverIP, &arg[2], sizeof(params.serverIP) - 1);
            params.connectMode = true;
            printf("Server IP: %s\n", params.serverIP);
        }
        else if (arg[0] == '/' && (arg[1] == 'p' || arg[1] == 'P'))
        {
            params.serverPort = atoi(&arg[2]);
            printf("Server Port: %d\n", params.serverPort);
        }
        else if (strcmp(arg, "connect") == 0)
        {
            params.connectMode = true;
        }
    }

    return true;
}

// Cross-platform main entry point
int main(int argc, char* argv[])
{
    printf("=================================================\n");
    printf("    MU Online Client - Cross-Platform Build\n");
    printf("=================================================\n\n");

    ConnectionParams connParams;
    if (!ParseCommandLine(argc, argv, connParams))
    {
        fprintf(stderr, "Failed to parse command line\n");
        return 1;
    }

    printf("Platform: %s\n", Platform::GetPlatformName());
    if (connParams.connectMode)
    {
        printf("Connect Mode: %s:%d\n", connParams.serverIP, connParams.serverPort);
    }
    printf("\n");

    printf("Initializing Platform Layer...\n");
    if (!Platform::Initialize())
    {
        fprintf(stderr, "Failed to initialize platform layer\n");
        return 1;
    }

    printf("Creating game window...\n");
    Platform::WindowDesc windowDesc;
    windowDesc.title = "MU Online Client";
    windowDesc.width = 1024;
    windowDesc.height = 768;
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
    printf("Window created successfully!\n\n");

    printf("Initializing game...\n");
    GameStubs::InitializeGame();
    printf("Game initialized!\n\n");

    printf("=================================================\n");
    printf("    Game Running - Press ESC to exit\n");
    printf("=================================================\n\n");

    int frameCount = 0;
    float lastTime = 0.0f;

    while (window->IsOpen())
    {
        window->ProcessEvents();

        float currentTime = frameCount * 0.016f;
        float deltaTime = 0.016f;

        GameStubs::UpdateGame(deltaTime);
        GameStubs::RenderGame();

        window->SwapBuffers();

        frameCount++;

        if (frameCount % 300 == 0)
        {
            printf("Frame %d - Game running...\n", frameCount);
        }
    }

    printf("\n=================================================\n");
    printf("    Shutting Down\n");
    printf("=================================================\n\n");

    printf("Shutting down game...\n");
    GameStubs::ShutdownGame();

    printf("Shutting down platform layer...\n");
    Platform::Shutdown();

    printf("\nMU Online Client terminated successfully.\n");
    return 0;
}
