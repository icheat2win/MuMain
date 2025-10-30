// GameGlobals.cpp - Global game variables for cross-platform builds
// This file defines the global variables that are normally scattered throughout the game code
// For cross-platform builds, we need to define them here to avoid linker errors

#ifdef USE_CROSSPLATFORM_MAIN

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cmath>
#include <string>

// OpenGL on macOS
#ifdef __APPLE__
    #include <OpenGL/gl.h>
#else
    #include <GL/gl.h>
#endif

// ================================================================
// TYPE DEFINITIONS (for cross-platform)
// ================================================================

#ifndef _WIN32
typedef void* HDC;
typedef void* HWND;
typedef uint32_t DWORD;
typedef uint8_t BYTE;
typedef uint16_t WORD;
#endif

// ================================================================
// WINDOW AND SCREEN VARIABLES
// ================================================================

int WindowWidth = 1024;
int WindowHeight = 768;
float g_fScreenRate_x = 1.6f;  // 1024/640
float g_fScreenRate_y = 1.6f;  // 768/480
bool g_bWndActive = true;
bool g_bUseWindowMode = true;

// ================================================================
// SCENE SYSTEM VARIABLES
// ================================================================

// Scene flags and states
enum EGameScene
{
    GAME_SCENE_NONE = 0,
    GAME_SCENE_LOGIN = 1,
    GAME_SCENE_CHARACTER = 2,
    GAME_SCENE_MAIN = 3,
    GAME_SCENE_LOADING = 4
};

EGameScene SceneFlag = GAME_SCENE_NONE;
int ErrorMessage = 0;
bool InitServerList = false;
bool InitLogIn = false;
bool InitLoading = false;
bool InitCharacterScene = false;
bool InitMainScene = false;
bool EnableMainRender = false;

// Network configuration
wchar_t g_lpszCmdURL[256] = L"127.0.0.1";
wchar_t* szServerIpAddress = g_lpszCmdURL;
unsigned short g_ServerPort = 44405;
int g_iLengthAuthorityCode = 0;

// ================================================================
// RENDERING SYSTEM VARIABLES
// ================================================================

// Camera and view matrices (will be properly initialized later)
float ViewMatrix[4][4] = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
float ProjectionMatrix[4][4] = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};

// ================================================================
// CHARACTER AND GAME DATA
// ================================================================

// These are large arrays that will be allocated in GameCore::PostInitialize()
// For now, declare them as null pointers
struct GATE_ATTRIBUTE;
struct SKILL_ATTRIBUTE;
struct ITEM_ATTRIBUTE;
struct CHARACTER;
struct CHARACTER_MACHINE;

GATE_ATTRIBUTE* GateAttribute = nullptr;
SKILL_ATTRIBUTE* SkillAttribute = nullptr;
ITEM_ATTRIBUTE* ItemAttribute = nullptr;
CHARACTER* CharactersClient = nullptr;
CHARACTER* Hero = nullptr;
CHARACTER_MACHINE* CharacterMachine = nullptr;
CHARACTER* CharacterAttribute = nullptr;

// ================================================================
// STUB IMPLEMENTATIONS
// ================================================================

// Helper function to set up 3D perspective
void SetupPerspective(float fov, float aspect, float nearPlane, float farPlane)
{
    float f = 1.0f / tanf(fov * 3.14159f / 360.0f);
    float matrix[16] = {
        f / aspect, 0, 0, 0,
        0, f, 0, 0,
        0, 0, (farPlane + nearPlane) / (nearPlane - farPlane), -1,
        0, 0, (2 * farPlane * nearPlane) / (nearPlane - farPlane), 0
    };
    glLoadMatrixf(matrix);
}

// From ZzzScene.cpp - enhanced rendering function with real OpenGL
void RenderScene(HDC hDC)
{
    static int frameCounter = 0;
    frameCounter++;
    float time = frameCounter * 0.016f;

    // Clear the screen with nice gradient color
    glClearColor(0.05f, 0.05f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // === 3D RENDERING SECTION ===
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // Set up 3D perspective
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspect = (float)WindowWidth / (float)WindowHeight;
    SetupPerspective(60.0f, aspect, 1.0f, 1000.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Set up camera (looking at origin from distance)
    float camX = sinf(time * 0.3f) * 5.0f;
    float camZ = cosf(time * 0.3f) * 5.0f;
    float camY = 3.0f;

    // Set up camera (rotate around scene)
    glTranslatef(0.0f, -1.0f, -7.0f);
    glRotatef(time * 20.0f, 0.0f, 1.0f, 0.0f);

    // Draw multiple colored cubes in a circle
    for (int i = 0; i < 6; i++)
    {
        glPushMatrix();

        float angle = (float)i / 6.0f * 6.28318f; // 2*PI
        float x = cosf(angle) * 2.0f;
        float z = sinf(angle) * 2.0f;

        glTranslatef(x, sinf(time * 2.0f + angle) * 0.5f, z);
        glRotatef(time * 50.0f + i * 60.0f, 1.0f, 1.0f, 0.0f);

        // Draw cube with colored faces
        glBegin(GL_QUADS);
            // Front face (Red)
            glColor3f(1.0f, 0.0f, 0.0f);
            glVertex3f(-0.5f, -0.5f,  0.5f);
            glVertex3f( 0.5f, -0.5f,  0.5f);
            glVertex3f( 0.5f,  0.5f,  0.5f);
            glVertex3f(-0.5f,  0.5f,  0.5f);

            // Back face (Green)
            glColor3f(0.0f, 1.0f, 0.0f);
            glVertex3f(-0.5f, -0.5f, -0.5f);
            glVertex3f(-0.5f,  0.5f, -0.5f);
            glVertex3f( 0.5f,  0.5f, -0.5f);
            glVertex3f( 0.5f, -0.5f, -0.5f);

            // Top face (Blue)
            glColor3f(0.0f, 0.0f, 1.0f);
            glVertex3f(-0.5f,  0.5f, -0.5f);
            glVertex3f(-0.5f,  0.5f,  0.5f);
            glVertex3f( 0.5f,  0.5f,  0.5f);
            glVertex3f( 0.5f,  0.5f, -0.5f);

            // Bottom face (Yellow)
            glColor3f(1.0f, 1.0f, 0.0f);
            glVertex3f(-0.5f, -0.5f, -0.5f);
            glVertex3f( 0.5f, -0.5f, -0.5f);
            glVertex3f( 0.5f, -0.5f,  0.5f);
            glVertex3f(-0.5f, -0.5f,  0.5f);

            // Right face (Magenta)
            glColor3f(1.0f, 0.0f, 1.0f);
            glVertex3f( 0.5f, -0.5f, -0.5f);
            glVertex3f( 0.5f,  0.5f, -0.5f);
            glVertex3f( 0.5f,  0.5f,  0.5f);
            glVertex3f( 0.5f, -0.5f,  0.5f);

            // Left face (Cyan)
            glColor3f(0.0f, 1.0f, 1.0f);
            glVertex3f(-0.5f, -0.5f, -0.5f);
            glVertex3f(-0.5f, -0.5f,  0.5f);
            glVertex3f(-0.5f,  0.5f,  0.5f);
            glVertex3f(-0.5f,  0.5f, -0.5f);
        glEnd();

        glPopMatrix();
    }

    // === 2D OVERLAY SECTION ===
    // Switch to 2D for UI elements
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, WindowWidth, WindowHeight, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_DEPTH_TEST);

    // Draw "MU ONLINE" text in top-left corner using lines
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINES);
        // M
        glVertex2f(20, 20);
        glVertex2f(20, 60);
        glVertex2f(20, 20);
        glVertex2f(30, 30);
        glVertex2f(30, 30);
        glVertex2f(40, 20);
        glVertex2f(40, 20);
        glVertex2f(40, 60);

        // U
        glVertex2f(50, 20);
        glVertex2f(50, 50);
        glVertex2f(50, 50);
        glVertex2f(55, 60);
        glVertex2f(55, 60);
        glVertex2f(60, 60);
        glVertex2f(60, 60);
        glVertex2f(65, 50);
        glVertex2f(65, 50);
        glVertex2f(65, 20);
    glEnd();

    // Draw FPS counter (simple)
    static float lastTime = 0.0f;
    static int fps = 60;
    if (time - lastTime >= 1.0f)
    {
        fps = frameCounter - (int)(lastTime / 0.016f);
        lastTime = time;

        if (frameCounter % 60 == 0)
        {
            printf("[RenderScene] Frame %d - FPS: %d - Real OpenGL rendering!\n",
                   frameCounter, fps);
        }
    }

    // Draw a simple FPS indicator (bar)
    float fpsRatio = (float)fps / 60.0f;
    if (fpsRatio > 1.0f) fpsRatio = 1.0f;

    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS);
        glVertex2f(WindowWidth - 110, 10);
        glVertex2f(WindowWidth - 110 + (100 * fpsRatio), 10);
        glVertex2f(WindowWidth - 110 + (100 * fpsRatio), 20);
        glVertex2f(WindowWidth - 110, 20);
    glEnd();

    // TODO: Eventually this will call the real rendering pipeline:
    // 1. Update camera (3D)
    // 2. Render terrain
    // 3. Render objects
    // 4. Render characters
    // 5. Render effects
    // 6. Render UI with real fonts
}

// Scene state update stub
void UpdateSceneState()
{
    // TODO: Update game state based on current scene
}

// Loading scene stub
void LoadingScene(HDC hDC)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // TODO: Show loading screen
}

// Frame timing stubs
bool CheckRenderNextFrame()
{
    // For now, always render
    return true;
}

void WaitForNextActivity(bool usePreciseSleep)
{
    // TODO: Implement frame rate limiting
}

// Name checking stub
bool CheckName()
{
    return true;
}

// ================================================================
// GLOBAL TEXT AND LOCALIZATION (stubs)
// ================================================================

wchar_t GlobalText[10000][100] = {L""};  // Simplified - actual size may vary
wchar_t g_aszMLSelection[32] = L"Eng";
std::wstring g_strSelectedML = L"Eng";

// ================================================================
// INPUT SYSTEM (stubs)
// ================================================================

// Mouse state
int MouseX = 0;
int MouseY = 0;
bool MouseLButton = false;
bool MouseRButton = false;
bool MouseMButton = false;

// Keyboard state
bool KeyState[256] = {false};

// ================================================================
// TODO: Add more as needed during integration
// ================================================================
//
// As we integrate more systems, we'll add:
// - UI system globals
// - Audio system globals
// - Effect system globals
// - Skill system globals
// - Item system globals
// - Map system globals
// etc.

#endif // USE_CROSSPLATFORM_MAIN
