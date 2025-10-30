# Migration Guide - Windows to Cross-Platform

This guide shows how to migrate existing Windows-specific code to use the cross-platform abstraction layer.

## Table of Contents

1. [Window Management](#window-management)
2. [Input Handling](#input-handling)
3. [File I/O](#file-io)
4. [Audio](#audio)
5. [Timing](#timing)
6. [Complete Example](#complete-example)

---

## Window Management

### Old Windows Code (Winmain.cpp)

```cpp
// Windows-specific window creation
WNDCLASS wc = {0};
wc.lpfnWndProc = WindowProc;
wc.hInstance = hInstance;
wc.lpszClassName = L"MuOnlineClass";
RegisterClass(&wc);

HWND hwnd = CreateWindowEx(
    0, L"MuOnlineClass", L"MU Online",
    WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, CW_USEDEFAULT,
    1024, 768,
    NULL, NULL, hInstance, NULL
);

ShowWindow(hwnd, SW_SHOW);

// Message loop
MSG msg;
while (GetMessage(&msg, NULL, 0, 0))
{
    TranslateMessage(&msg);
    DispatchMessage(&msg);
}
```

### New Cross-Platform Code

```cpp
#include "Platform/Platform.h"

// Initialize platform layer
if (!Platform::Initialize())
{
    return 1;
}

// Create window
Platform::WindowDesc windowDesc;
windowDesc.title = "MU Online";
windowDesc.width = 1024;
windowDesc.height = 768;
windowDesc.fullscreen = false;
windowDesc.resizable = true;
windowDesc.vsync = true;

if (!Platform::WindowManager::CreateMainWindow(windowDesc))
{
    Platform::Shutdown();
    return 1;
}

Platform::Window* window = Platform::WindowManager::GetMainWindow();

// Main loop
while (window->IsOpen())
{
    // Process window events
    window->ProcessEvents();

    // Your game loop here
    // ...

    // Swap buffers
    window->SwapBuffers();
}

// Cleanup
Platform::Shutdown();
```

### OpenGL Context

**Old Windows Code:**

```cpp
PIXELFORMATDESCRIPTOR pfd = {0};
pfd.nSize = sizeof(pfd);
pfd.nVersion = 1;
pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
pfd.iPixelType = PFD_TYPE_RGBA;
pfd.cColorBits = 32;
pfd.cDepthBits = 24;

HDC hdc = GetDC(hwnd);
int pixelFormat = ChoosePixelFormat(hdc, &pfd);
SetPixelFormat(hdc, pixelFormat, &pfd);

HGLRC hglrc = wglCreateContext(hdc);
wglMakeCurrent(hdc, hglrc);
```

**New Cross-Platform Code:**

```cpp
// OpenGL context is created automatically with the window
// Just make it current if needed
window->MakeContextCurrent();

// That's it! GLFW handles all the complexity
```

---

## Input Handling

### Keyboard Input

**Old Windows Code:**

```cpp
// Check if key is pressed
if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
{
    // Escape key pressed
}

if (GetAsyncKeyState(VK_LEFT) & 0x8000)
{
    // Left arrow pressed
}

if (GetAsyncKeyState('W') & 0x8000)
{
    // W key pressed
}
```

**New Cross-Platform Code:**

```cpp
#include "Platform/PlatformInput.h"

// Update input state (call once per frame)
Platform::Input::Update();

// Check if key is pressed
if (Platform::Input::IsKeyDown(Platform::KeyCode::Escape))
{
    // Escape key pressed
}

if (Platform::Input::IsKeyDown(Platform::KeyCode::Left))
{
    // Left arrow pressed
}

if (Platform::Input::IsKeyDown(Platform::KeyCode::W))
{
    // W key pressed
}

// Check if key was just pressed this frame
if (Platform::Input::IsKeyPressed(Platform::KeyCode::Space))
{
    // Space bar just pressed
}
```

### Mouse Input

**Old Windows Code:**

```cpp
POINT pt;
GetCursorPos(&pt);
ScreenToClient(hwnd, &pt);
int mouseX = pt.x;
int mouseY = pt.y;

if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
{
    // Left mouse button pressed
}
```

**New Cross-Platform Code:**

```cpp
// Get mouse position
int mouseX, mouseY;
Platform::Input::GetMousePosition(mouseX, mouseY);

// Check mouse buttons
if (Platform::Input::IsMouseButtonDown(Platform::MouseButton::Left))
{
    // Left mouse button pressed
}

// Get mouse movement
int deltaX, deltaY;
Platform::Input::GetMouseDelta(deltaX, deltaY);
```

---

## File I/O

### Reading Files

**Old Windows Code:**

```cpp
HANDLE hFile = CreateFile(
    L"data.bin",
    GENERIC_READ,
    FILE_SHARE_READ,
    NULL,
    OPEN_EXISTING,
    FILE_ATTRIBUTE_NORMAL,
    NULL
);

if (hFile == INVALID_HANDLE_VALUE)
{
    // Error
}

char buffer[1024];
DWORD bytesRead;
ReadFile(hFile, buffer, sizeof(buffer), &bytesRead, NULL);

CloseHandle(hFile);
```

**New Cross-Platform Code:**

```cpp
#include "Platform/PlatformFile.h"

Platform::File file;
if (!file.Open("data.bin", Platform::FileAccess::Read, Platform::FileMode::OpenExisting))
{
    // Error
}

char buffer[1024];
size_t bytesRead = file.Read(buffer, sizeof(buffer));

file.Close();
```

### Writing Files

**Old Windows Code:**

```cpp
HANDLE hFile = CreateFile(
    L"output.txt",
    GENERIC_WRITE,
    0,
    NULL,
    CREATE_ALWAYS,
    FILE_ATTRIBUTE_NORMAL,
    NULL
);

DWORD bytesWritten;
WriteFile(hFile, data, dataSize, &bytesWritten, NULL);

CloseHandle(hFile);
```

**New Cross-Platform Code:**

```cpp
Platform::File file;
if (!file.Open("output.txt", Platform::FileAccess::Write, Platform::FileMode::CreateAlways))
{
    // Error
}

size_t bytesWritten = file.Write(data, dataSize);

file.Close();
```

### File Utilities

**Old Windows Code:**

```cpp
// Check if file exists
DWORD attrib = GetFileAttributes(L"file.txt");
bool exists = (attrib != INVALID_FILE_ATTRIBUTES &&
               !(attrib & FILE_ATTRIBUTE_DIRECTORY));

// Get file size
LARGE_INTEGER fileSize;
GetFileSizeEx(hFile, &fileSize);

// Delete file
DeleteFile(L"file.txt");
```

**New Cross-Platform Code:**

```cpp
// Check if file exists
bool exists = Platform::FileSystem::FileExists("file.txt");

// Get file size
int64_t fileSize = Platform::FileSystem::GetFileSize("file.txt");

// Delete file
Platform::FileSystem::DeleteFile("file.txt");

// Create directory
Platform::FileSystem::CreateDirectory("data/maps");

// Get current directory
char currentDir[512];
Platform::FileSystem::GetCurrentDirectory(currentDir, sizeof(currentDir));
```

---

## Audio

### Playing Sounds

**Old Windows Code (using wzAudio/DirectSound):**

```cpp
// Load sound
LPDIRECTSOUNDBUFFER soundBuffer;
// ... complex DirectSound setup ...

// Play sound
soundBuffer->Play(0, 0, 0);
```

**New Cross-Platform Code:**

```cpp
#include "Platform/PlatformAudio.h"

// Initialize audio
Platform::Audio::Initialize();

// Load sound
Platform::AudioBufferHandle buffer = Platform::Audio::CreateBufferFromFile("sound.wav");

// Play sound
Platform::Audio::PlaySound(buffer, 1.0f);  // volume 1.0

// Play 3D sound
Platform::Audio::PlaySound3D(buffer, x, y, z, 1.0f);

// Cleanup
Platform::Audio::DestroyBuffer(buffer);
Platform::Audio::Shutdown();
```

---

## Timing

### High-Resolution Timer

**Old Windows Code:**

```cpp
LARGE_INTEGER frequency, startTime, currentTime;
QueryPerformanceFrequency(&frequency);
QueryPerformanceCounter(&startTime);

// Later...
QueryPerformanceCounter(&currentTime);
double elapsedSeconds = (double)(currentTime.QuadPart - startTime.QuadPart) / frequency.QuadPart;
```

**New Cross-Platform Code:**

```cpp
#include "Platform/PlatformTime.h"

Platform::Timer timer;
timer.Start();

// Later...
double elapsedSeconds = timer.GetElapsedSeconds();
```

### Frame Timing

**Old Windows Code:**

```cpp
DWORD lastTime = GetTickCount();
float deltaTime;

while (running)
{
    DWORD currentTime = GetTickCount();
    deltaTime = (currentTime - lastTime) / 1000.0f;
    lastTime = currentTime;

    // Game update
}
```

**New Cross-Platform Code:**

```cpp
Platform::Time::FrameTimer frameTimer;

while (running)
{
    frameTimer.Update();

    float deltaTime = frameTimer.GetDeltaTime();
    float fps = frameTimer.GetFPS();

    // Game update
}
```

---

## Complete Example

### Migrating Winmain.cpp

Here's a complete example showing how to migrate the main entry point:

**Old Winmain.cpp (Simplified):**

```cpp
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow)
{
    // Register window class
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"MuOnlineClass";
    RegisterClass(&wc);

    // Create window
    HWND hwnd = CreateWindow(wc.lpszClassName, L"MU Online",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        1024, 768, NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, SW_SHOW);

    // Setup OpenGL
    HDC hdc = GetDC(hwnd);
    // ... pixel format setup ...
    HGLRC hglrc = wglCreateContext(hdc);
    wglMakeCurrent(hdc, hglrc);

    // Initialize game
    InitializeGame();

    // Message loop
    MSG msg;
    DWORD lastTime = GetTickCount();

    while (true)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                break;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            // Calculate delta time
            DWORD currentTime = GetTickCount();
            float deltaTime = (currentTime - lastTime) / 1000.0f;
            lastTime = currentTime;

            // Update game
            UpdateGame(deltaTime);

            // Render
            RenderGame();

            // Swap buffers
            SwapBuffers(hdc);
        }
    }

    // Cleanup
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hglrc);
    ReleaseDC(hwnd, hdc);
    DestroyWindow(hwnd);

    return (int)msg.wParam;
}
```

**New Cross-Platform Version:**

```cpp
#include "Platform/Platform.h"
#include "Platform/PlatformWindow.h"
#include "Platform/PlatformInput.h"
#include "Platform/PlatformTime.h"

// Cross-platform entry point
int main(int argc, char* argv[])
{
    // Initialize platform layer
    if (!Platform::Initialize())
    {
        fprintf(stderr, "Failed to initialize platform\n");
        return 1;
    }

    // Create window
    Platform::WindowDesc windowDesc;
    windowDesc.title = "MU Online";
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

    // OpenGL context is created automatically with window
    window->MakeContextCurrent();

    // Initialize game
    InitializeGame();

    // Frame timer
    Platform::Time::FrameTimer frameTimer;

    // Main loop
    while (window->IsOpen())
    {
        // Update frame timer
        frameTimer.Update();
        float deltaTime = frameTimer.GetDeltaTime();

        // Process window events
        window->ProcessEvents();

        // Update input
        Platform::Input::Update();

        // Check for quit (Escape key)
        if (Platform::Input::IsKeyPressed(Platform::KeyCode::Escape))
        {
            window->SetShouldClose(true);
        }

        // Update game
        UpdateGame(deltaTime);

        // Render
        RenderGame();

        // Swap buffers
        window->SwapBuffers();
    }

    // Cleanup
    Platform::Shutdown();

    return 0;
}
```

---

## Migration Checklist

Use this checklist when migrating code:

### Window Management
- [ ] Replace `CreateWindow` with `Platform::WindowManager::CreateMainWindow()`
- [ ] Replace `ShowWindow` with `window->Show()`
- [ ] Replace `DestroyWindow` with `window->Destroy()`
- [ ] Replace message loop with `window->ProcessEvents()`
- [ ] Replace `wglCreateContext` with automatic context creation

### Input
- [ ] Replace `GetAsyncKeyState` with `Platform::Input::IsKeyDown()`
- [ ] Replace `GetCursorPos` with `Platform::Input::GetMousePosition()`
- [ ] Add `Platform::Input::Update()` call in main loop

### File I/O
- [ ] Replace `CreateFile` with `Platform::File::Open()`
- [ ] Replace `ReadFile` with `file.Read()`
- [ ] Replace `WriteFile` with `file.Write()`
- [ ] Replace `CloseHandle` with `file.Close()`
- [ ] Replace `GetFileAttributes` with `Platform::FileSystem::FileExists()`

### Audio
- [ ] Replace DirectSound/wzAudio with `Platform::Audio`
- [ ] Use `Platform::Audio::CreateBufferFromFile()` for loading
- [ ] Use `Platform::Audio::PlaySound()` for playback

### Timing
- [ ] Replace `QueryPerformanceCounter` with `Platform::Timer`
- [ ] Replace `GetTickCount` with `Platform::Time::GetTickCount()`
- [ ] Use `Platform::Time::FrameTimer` for frame timing

### Entry Point
- [ ] Change `WinMain` to `main(int argc, char* argv[])`
- [ ] Add `Platform::Initialize()` at start
- [ ] Add `Platform::Shutdown()` at end

---

## Testing Strategy

1. **Start with Windows**
   - Build with `-DUSE_GLFW=ON` to test cross-platform code on Windows
   - Verify everything works before moving to other platforms

2. **Test Each Subsystem**
   - Test window creation and event handling
   - Test input (keyboard and mouse)
   - Test file I/O
   - Test audio playback

3. **Port to Linux/macOS**
   - Once Windows GLFW build works, test on Linux
   - Then test on macOS
   - Fix platform-specific issues

---

## Common Pitfalls

### 1. **Path Separators**
Windows uses `\`, Unix uses `/`. Always use `/` or `Platform::FileSystem::NormalizePath()`.

```cpp
// Bad
file.Open("data\\textures\\player.jpg", ...);

// Good
file.Open("data/textures/player.jpg", ...);
```

### 2. **Case Sensitivity**
Linux/macOS filesystems are case-sensitive. Use consistent casing.

```cpp
// May work on Windows but fail on Linux
file.Open("Data/Texture.jpg", ...);  // Directory is actually "data"
```

### 3. **Calling Order**
Initialize platform before creating windows:

```cpp
// Bad
auto window = new Platform::Window();
Platform::Initialize();  // Too late!

// Good
Platform::Initialize();
auto window = new Platform::Window();
```

### 4. **Input Update**
Must call `Platform::Input::Update()` once per frame:

```cpp
while (window->IsOpen())
{
    window->ProcessEvents();
    Platform::Input::Update();  // Don't forget!

    // Now check input
    if (Platform::Input::IsKeyDown(...)) { }
}
```

---

## Next Steps

1. Start with `source/Winmain.cpp` - replace the main entry point
2. Update `source/Input.cpp` - replace input handling
3. Update file I/O throughout the codebase
4. Update audio code
5. Test on Windows with GLFW
6. Test on Linux
7. Test on macOS

See **[CROSSPLATFORM.md](CROSSPLATFORM.md)** for the complete implementation roadmap.

---

## Questions?

If you run into issues during migration:

1. Check this guide for common patterns
2. Check **[CROSSPLATFORM.md](CROSSPLATFORM.md)** for architecture details
3. Look at the platform abstraction headers for API details
4. Create an issue on GitHub

Good luck with the migration!
