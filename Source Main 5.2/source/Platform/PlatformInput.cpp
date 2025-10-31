#include "PlatformInput.h"
#include "Platform.h"

#if !defined(USE_GLFW)

namespace Platform
{
    bool Input::Initialize()
    {
        return true;
    }

    void Input::Shutdown()
    {
    }

    void Input::Update()
    {
    }

    bool Input::IsKeyDown(KeyCode)
    {
        return false;
    }

    bool Input::IsKeyUp(KeyCode)
    {
        return true;
    }

    bool Input::IsKeyPressed(KeyCode)
    {
        return false;
    }

    bool Input::IsKeyReleased(KeyCode)
    {
        return false;
    }

    bool Input::IsMouseButtonDown(MouseButton)
    {
        return false;
    }

    bool Input::IsMouseButtonUp(MouseButton)
    {
        return true;
    }

    bool Input::IsMouseButtonPressed(MouseButton)
    {
        return false;
    }

    bool Input::IsMouseButtonReleased(MouseButton)
    {
        return false;
    }

    void Input::GetMousePosition(int& x, int& y)
    {
        x = 0;
        y = 0;
    }

    void Input::SetMousePosition(int, int)
    {
    }

    void Input::GetMouseDelta(int& dx, int& dy)
    {
        dx = 0;
        dy = 0;
    }

    float Input::GetMouseWheelDelta()
    {
        return 0.0f;
    }

    const char* Input::GetClipboardText()
    {
        return "";
    }

    void Input::SetClipboardText(const char*)
    {
    }

    void Input::EnableTextInput()
    {
    }

    void Input::DisableTextInput()
    {
    }

    const char* Input::GetInputText()
    {
        return "";
    }

    void Input::ShowCursor()
    {
    }

    void Input::HideCursor()
    {
    }

    bool Input::IsCursorVisible()
    {
        return true;
    }
}

#endif // !defined(USE_GLFW)
