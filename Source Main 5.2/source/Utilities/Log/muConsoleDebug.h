#pragma once

#include <cstdio>
#include <cstdarg>

// Stub implementation for console debug logging
class CmuConsoleDebug
{
public:
    enum eMode
    {
        MCD_NORMAL = 0,
        MCD_ERROR = 1,
        MCD_SEND = 2,
        MCD_RECEIVE = 3
    };

    CmuConsoleDebug() {}
    ~CmuConsoleDebug() {}

    void Write(eMode mode, const wchar_t* format, ...)
    {
#ifdef _DEBUG
        wchar_t buffer[2048];
        va_list args;
        va_start(args, format);
        vswprintf_s(buffer, 2048, format, args);
        va_end(args);
        
        OutputDebugStringW(L"[MU] ");
        OutputDebugStringW(buffer);
        OutputDebugStringW(L"\n");
#endif
    }

    void Write(const wchar_t* format, ...)
    {
#ifdef _DEBUG
        wchar_t buffer[2048];
        va_list args;
        va_start(args, format);
        vswprintf_s(buffer, 2048, format, args);
        va_end(args);
        
        OutputDebugStringW(L"[MU] ");
        OutputDebugStringW(buffer);
        OutputDebugStringW(L"\n");
#endif
    }
};

// Global instance
inline CmuConsoleDebug* g_ConsoleDebug = new CmuConsoleDebug();
