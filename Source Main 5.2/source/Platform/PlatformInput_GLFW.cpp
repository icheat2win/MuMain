#ifdef USE_GLFW

#include "PlatformInput.h"
#include "PlatformWindow.h"
#include <GLFW/glfw3.h>

#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <unordered_map>
#include <vector>

namespace Platform
{
    namespace
    {
        constexpr int MaxVirtualKeys = 512;
        constexpr int MaxMouseButtons = 8;

        GLFWwindow* s_Window = nullptr;

        std::array<bool, MaxVirtualKeys> s_CurrentKeyDown{};
        std::array<bool, MaxVirtualKeys> s_PreviousKeyDown{};
        std::array<InputState, MaxVirtualKeys> s_KeyStates{};

        std::array<bool, MaxMouseButtons> s_CurrentMouseDown{};
        std::array<bool, MaxMouseButtons> s_PreviousMouseDown{};
        std::array<InputState, MaxMouseButtons> s_MouseStates{};

        double s_MouseX = 0.0;
        double s_MouseY = 0.0;
        double s_PreviousMouseX = 0.0;
        double s_PreviousMouseY = 0.0;
        float s_MouseWheelDelta = 0.0f;
        float s_PendingScrollDelta = 0.0f;
        bool s_TextInputEnabled = false;
        std::string s_TextInputBuffer;

        std::unordered_map<int, int> s_VirtualKeyToGlfw;

        std::array<int, MaxVirtualKeys> s_GlfwKeyForVirtualKey{};

        std::array<int, MaxMouseButtons> s_GlfwMouseButtonForIndex = {
            GLFW_MOUSE_BUTTON_LEFT,
            GLFW_MOUSE_BUTTON_RIGHT,
            GLFW_MOUSE_BUTTON_MIDDLE,
            GLFW_MOUSE_BUTTON_4,
            GLFW_MOUSE_BUTTON_5,
            GLFW_MOUSE_BUTTON_6,
            GLFW_MOUSE_BUTTON_7,
            GLFW_MOUSE_BUTTON_8
        };

        void InitializeKeyMap()
        {
            if (!s_VirtualKeyToGlfw.empty())
            {
                return;
            }

            auto mapKey = [](int vk, int glfwKey)
            {
                s_VirtualKeyToGlfw[vk] = glfwKey;
                if (vk >= 0 && vk < MaxVirtualKeys)
                {
                    s_GlfwKeyForVirtualKey[vk] = glfwKey;
                }
            };

            // Digits 0-9
            for (int i = 0; i <= 9; ++i)
            {
                mapKey(0x30 + i, GLFW_KEY_0 + i);
            }

            // Letters A-Z
            for (int i = 0; i < 26; ++i)
            {
                mapKey(0x41 + i, GLFW_KEY_A + i);
            }

            mapKey(VK_ESCAPE, GLFW_KEY_ESCAPE);
            mapKey(VK_SPACE, GLFW_KEY_SPACE);
            mapKey(VK_RETURN, GLFW_KEY_ENTER);
            mapKey(VK_BACK, GLFW_KEY_BACKSPACE);
            mapKey(VK_TAB, GLFW_KEY_TAB);
            mapKey(VK_DELETE, GLFW_KEY_DELETE);
            mapKey(VK_INSERT, GLFW_KEY_INSERT);
            mapKey(VK_HOME, GLFW_KEY_HOME);
            mapKey(VK_END, GLFW_KEY_END);
            mapKey(VK_PRIOR, GLFW_KEY_PAGE_UP);
            mapKey(VK_NEXT, GLFW_KEY_PAGE_DOWN);
            mapKey(VK_LEFT, GLFW_KEY_LEFT);
            mapKey(VK_RIGHT, GLFW_KEY_RIGHT);
            mapKey(VK_UP, GLFW_KEY_UP);
            mapKey(VK_DOWN, GLFW_KEY_DOWN);
            mapKey(VK_SHIFT, GLFW_KEY_LEFT_SHIFT);
            mapKey(VK_LSHIFT, GLFW_KEY_LEFT_SHIFT);
            mapKey(VK_RSHIFT, GLFW_KEY_RIGHT_SHIFT);
            mapKey(VK_CONTROL, GLFW_KEY_LEFT_CONTROL);
            mapKey(VK_LCONTROL, GLFW_KEY_LEFT_CONTROL);
            mapKey(VK_RCONTROL, GLFW_KEY_RIGHT_CONTROL);
            mapKey(VK_MENU, GLFW_KEY_LEFT_ALT);
            mapKey(VK_LMENU, GLFW_KEY_LEFT_ALT);
            mapKey(VK_RMENU, GLFW_KEY_RIGHT_ALT);
            mapKey(VK_CAPITAL, GLFW_KEY_CAPS_LOCK);

            mapKey(VK_F1, GLFW_KEY_F1);
            mapKey(VK_F2, GLFW_KEY_F2);
            mapKey(VK_F3, GLFW_KEY_F3);
            mapKey(VK_F4, GLFW_KEY_F4);
            mapKey(VK_F5, GLFW_KEY_F5);
            mapKey(VK_F6, GLFW_KEY_F6);
            mapKey(VK_F7, GLFW_KEY_F7);
            mapKey(VK_F8, GLFW_KEY_F8);
            mapKey(VK_F9, GLFW_KEY_F9);
            mapKey(VK_F10, GLFW_KEY_F10);
            mapKey(VK_F11, GLFW_KEY_F11);
            mapKey(VK_F12, GLFW_KEY_F12);
        }

        InputState CalculateState(bool previousDown, bool currentDown)
        {
            if (currentDown)
            {
                return previousDown ? InputState::Held : InputState::Pressed;
            }

            return previousDown ? InputState::Released : InputState::Released;
        }

        void EnsureWindow()
        {
            if (!s_Window)
            {
                auto* window = WindowManager::GetMainWindow();
                if (window != nullptr)
                {
                    s_Window = static_cast<GLFWwindow*>(window->GetHandle());
                    if (s_Window)
                    {
                        glfwGetCursorPos(s_Window, &s_MouseX, &s_MouseY);
                        s_PreviousMouseX = s_MouseX;
                        s_PreviousMouseY = s_MouseY;
                    }
                }
            }
        }

        void ResetTransientState()
        {
            s_TextInputBuffer.clear();
            s_PendingScrollDelta = 0.0f;
        }

        void ScrollCallback(GLFWwindow*, double, double yoffset)
        {
            s_PendingScrollDelta += static_cast<float>(yoffset);
        }

        std::string Utf8FromCodepoint(unsigned int codepoint)
        {
            std::string utf8;
            if (codepoint <= 0x7F)
            {
                utf8.push_back(static_cast<char>(codepoint));
            }
            else if (codepoint <= 0x7FF)
            {
                utf8.push_back(static_cast<char>(0xC0 | ((codepoint >> 6) & 0x1F)));
                utf8.push_back(static_cast<char>(0x80 | (codepoint & 0x3F)));
            }
            else if (codepoint <= 0xFFFF)
            {
                utf8.push_back(static_cast<char>(0xE0 | ((codepoint >> 12) & 0x0F)));
                utf8.push_back(static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F)));
                utf8.push_back(static_cast<char>(0x80 | (codepoint & 0x3F)));
            }
            else if (codepoint <= 0x10FFFF)
            {
                utf8.push_back(static_cast<char>(0xF0 | ((codepoint >> 18) & 0x07)));
                utf8.push_back(static_cast<char>(0x80 | ((codepoint >> 12) & 0x3F)));
                utf8.push_back(static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F)));
                utf8.push_back(static_cast<char>(0x80 | (codepoint & 0x3F)));
            }
            return utf8;
        }

        void CharCallback(GLFWwindow*, unsigned int codepoint)
        {
            if (!s_TextInputEnabled)
            {
                return;
            }

            auto utf8 = Utf8FromCodepoint(codepoint);
            s_TextInputBuffer.append(utf8);
        }
    }

    bool Input::Initialize()
    {
        InitializeKeyMap();
        s_CurrentKeyDown.fill(false);
        s_PreviousKeyDown.fill(false);
        s_KeyStates.fill(InputState::Released);
        s_CurrentMouseDown.fill(false);
        s_PreviousMouseDown.fill(false);
        s_MouseStates.fill(InputState::Released);
        s_MouseX = s_MouseY = 0.0;
        s_PreviousMouseX = s_PreviousMouseY = 0.0;
        s_MouseWheelDelta = 0.0f;
        s_PendingScrollDelta = 0.0f;
        s_TextInputEnabled = false;
        s_TextInputBuffer.clear();

        EnsureWindow();
        if (s_Window)
        {
            glfwSetCharCallback(s_Window, CharCallback);
            glfwSetScrollCallback(s_Window, ScrollCallback);
        }

        return true;
    }

    void Input::Shutdown()
    {
        if (s_Window)
        {
            glfwSetCharCallback(s_Window, nullptr);
            glfwSetScrollCallback(s_Window, nullptr);
        }

        s_Window = nullptr;
        s_TextInputBuffer.clear();
    }

    void Input::Update()
    {
        EnsureWindow();
        if (!s_Window)
        {
            return;
        }

        ResetTransientState();

        s_PreviousKeyDown = s_CurrentKeyDown;
        s_PreviousMouseDown = s_CurrentMouseDown;

        for (const auto& pair : s_VirtualKeyToGlfw)
        {
            int vk = pair.first;
            int glfwKey = pair.second;

            int state = glfwGetKey(s_Window, glfwKey);
            bool isDown = state == GLFW_PRESS || state == GLFW_REPEAT;
            s_CurrentKeyDown[vk] = isDown;
            s_KeyStates[vk] = CalculateState(s_PreviousKeyDown[vk], isDown);
        }

        for (size_t i = 0; i < s_GlfwMouseButtonForIndex.size(); ++i)
        {
            int glfwButton = s_GlfwMouseButtonForIndex[i];
            int state = glfwGetMouseButton(s_Window, glfwButton);
            bool isDown = state == GLFW_PRESS;
            s_CurrentMouseDown[i] = isDown;
            s_MouseStates[i] = CalculateState(s_PreviousMouseDown[i], isDown);
        }

        double cursorX = 0.0;
        double cursorY = 0.0;
        glfwGetCursorPos(s_Window, &cursorX, &cursorY);

        s_PreviousMouseX = s_MouseX;
        s_PreviousMouseY = s_MouseY;
        s_MouseX = cursorX;
        s_MouseY = cursorY;

        s_MouseWheelDelta = s_PendingScrollDelta;
        s_PendingScrollDelta = 0.0f;
    }

    bool Input::IsKeyDown(KeyCode key)
    {
        int index = static_cast<int>(key);
        if (index < 0 || index >= MaxVirtualKeys)
        {
            return false;
        }
        return s_CurrentKeyDown[index];
    }

    bool Input::IsKeyUp(KeyCode key)
    {
        return !IsKeyDown(key);
    }

    bool Input::IsKeyPressed(KeyCode key)
    {
        int index = static_cast<int>(key);
        if (index < 0 || index >= MaxVirtualKeys)
        {
            return false;
        }
        return s_KeyStates[index] == InputState::Pressed;
    }

    bool Input::IsKeyReleased(KeyCode key)
    {
        int index = static_cast<int>(key);
        if (index < 0 || index >= MaxVirtualKeys)
        {
            return false;
        }
        return s_KeyStates[index] == InputState::Released && s_PreviousKeyDown[index];
    }

    bool Input::IsMouseButtonDown(MouseButton button)
    {
        int index = static_cast<int>(button);
        if (index < 0 || index >= static_cast<int>(s_CurrentMouseDown.size()))
        {
            return false;
        }
        return s_CurrentMouseDown[index];
    }

    bool Input::IsMouseButtonUp(MouseButton button)
    {
        return !IsMouseButtonDown(button);
    }

    bool Input::IsMouseButtonPressed(MouseButton button)
    {
        int index = static_cast<int>(button);
        if (index < 0 || index >= static_cast<int>(s_MouseStates.size()))
        {
            return false;
        }
        return s_MouseStates[index] == InputState::Pressed;
    }

    bool Input::IsMouseButtonReleased(MouseButton button)
    {
        int index = static_cast<int>(button);
        if (index < 0 || index >= static_cast<int>(s_MouseStates.size()))
        {
            return false;
        }
        return s_MouseStates[index] == InputState::Released && s_PreviousMouseDown[index];
    }

    void Input::GetMousePosition(int& x, int& y)
    {
        x = static_cast<int>(s_MouseX);
        y = static_cast<int>(s_MouseY);
    }

    void Input::SetMousePosition(int x, int y)
    {
        EnsureWindow();
        if (!s_Window)
        {
            return;
        }

        glfwSetCursorPos(s_Window, static_cast<double>(x), static_cast<double>(y));
        s_MouseX = static_cast<double>(x);
        s_MouseY = static_cast<double>(y);
    }

    void Input::GetMouseDelta(int& dx, int& dy)
    {
        dx = static_cast<int>(s_MouseX - s_PreviousMouseX);
        dy = static_cast<int>(s_MouseY - s_PreviousMouseY);
    }

    float Input::GetMouseWheelDelta()
    {
        return s_MouseWheelDelta;
    }

    const char* Input::GetClipboardText()
    {
        EnsureWindow();
        if (!s_Window)
        {
            return nullptr;
        }

        const char* text = glfwGetClipboardString(s_Window);
        return text != nullptr ? text : "";
    }

    void Input::SetClipboardText(const char* text)
    {
        EnsureWindow();
        if (!s_Window || !text)
        {
            return;
        }

        glfwSetClipboardString(s_Window, text);
    }

    void Input::EnableTextInput()
    {
        s_TextInputEnabled = true;
    }

    void Input::DisableTextInput()
    {
        s_TextInputEnabled = false;
    }

    const char* Input::GetInputText()
    {
        return s_TextInputBuffer.c_str();
    }

    void Input::ShowCursor()
    {
        EnsureWindow();
        if (!s_Window)
        {
            return;
        }

        glfwSetInputMode(s_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    void Input::HideCursor()
    {
        EnsureWindow();
        if (!s_Window)
        {
            return;
        }

        glfwSetInputMode(s_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    bool Input::IsCursorVisible()
    {
        EnsureWindow();
        if (!s_Window)
        {
            return true;
        }

        int mode = glfwGetInputMode(s_Window, GLFW_CURSOR);
        return mode != GLFW_CURSOR_DISABLED;
    }
}

#endif // USE_GLFW
