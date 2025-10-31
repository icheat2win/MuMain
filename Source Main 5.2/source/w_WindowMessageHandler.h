#pragma once

namespace util
{
    struct WindowMessageHandler
    {
        virtual ~WindowMessageHandler() = default;
        virtual bool HandleWindowMessage(UINT message, WPARAM wParam, LPARAM lParam, LRESULT& result) = 0;
    };
};
