// PlatformInput.h - Cross-platform input handling
// Replaces GetAsyncKeyState and other Windows-specific input functions

#pragma once

#include <cstdint>

namespace Platform
{
    // Key codes (mapped to common values across platforms)
    enum class KeyCode : int
    {
        Unknown = -1,

        // Alphanumeric
        A = 65, B = 66, C = 67, D = 68, E = 69, F = 70, G = 71, H = 72,
        I = 73, J = 74, K = 75, L = 76, M = 77, N = 78, O = 79, P = 80,
        Q = 81, R = 82, S = 83, T = 84, U = 85, V = 86, W = 87, X = 88,
        Y = 89, Z = 90,

        // Numbers
        Num0 = 48, Num1 = 49, Num2 = 50, Num3 = 51, Num4 = 52,
        Num5 = 53, Num6 = 54, Num7 = 55, Num8 = 56, Num9 = 57,

        // Function keys
        F1 = 112, F2 = 113, F3 = 114, F4 = 115, F5 = 116, F6 = 117,
        F7 = 118, F8 = 119, F9 = 120, F10 = 121, F11 = 122, F12 = 123,

        // Special keys
        Escape = 27,
        Enter = 13,
        Tab = 9,
        Backspace = 8,
        Space = 32,

        // Arrow keys
        Left = 37,
        Up = 38,
        Right = 39,
        Down = 40,

        // Modifiers
        Shift = 16,
        Control = 17,
        Alt = 18,

        // Other
        Insert = 45,
        Delete = 46,
        Home = 36,
        End = 35,
        PageUp = 33,
        PageDown = 34,

        // Mouse buttons (for compatibility)
        MouseLeft = 1,
        MouseRight = 2,
        MouseMiddle = 4
    };

    // Mouse button codes
    enum class MouseButton : int
    {
        Left = 0,
        Right = 1,
        Middle = 2,
        Button4 = 3,
        Button5 = 4
    };

    // Input state
    enum class InputState : uint8_t
    {
        Released = 0,
        Pressed = 1,
        Held = 2
    };

    // Input manager
    class Input
    {
    public:
        // Initialize input system
        static bool Initialize();

        // Shutdown input system
        static void Shutdown();

        // Update input state (call once per frame)
        static void Update();

        // Keyboard input
        static bool IsKeyDown(KeyCode key);
        static bool IsKeyUp(KeyCode key);
        static bool IsKeyPressed(KeyCode key);   // Just pressed this frame
        static bool IsKeyReleased(KeyCode key);  // Just released this frame

        // Mouse input
        static bool IsMouseButtonDown(MouseButton button);
        static bool IsMouseButtonUp(MouseButton button);
        static bool IsMouseButtonPressed(MouseButton button);
        static bool IsMouseButtonReleased(MouseButton button);

        // Mouse position
        static void GetMousePosition(int& x, int& y);
        static void SetMousePosition(int x, int y);
        static void GetMouseDelta(int& dx, int& dy);

        // Mouse wheel
        static float GetMouseWheelDelta();

        // Clipboard
        static const char* GetClipboardText();
        static void SetClipboardText(const char* text);

        // Text input (for chat, etc.)
        static void EnableTextInput();
        static void DisableTextInput();
        static const char* GetInputText();  // Get text entered this frame

        // Show/Hide cursor
        static void ShowCursor();
        static void HideCursor();
        static bool IsCursorVisible();

    private:
        Input() = delete;
    };

    // Helper function to convert Windows VK codes to KeyCode
    #if PLATFORM_WINDOWS
    KeyCode VKToKeyCode(int vk);
    int KeyCodeToVK(KeyCode key);
    #endif
}
