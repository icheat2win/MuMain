#pragma once

#include <cstdio>
#include <cstdarg>

// Stub implementation for error reporting
class CErrorReport
{
public:
    CErrorReport() {}
    ~CErrorReport() {}

    void Write(const wchar_t* format, ...)
    {
#ifdef _DEBUG
        wchar_t buffer[2048];
        va_list args;
        va_start(args, format);
        vswprintf_s(buffer, 2048, format, args);
        va_end(args);
        
        OutputDebugStringW(L"[ERROR] ");
        OutputDebugStringW(buffer);
        OutputDebugStringW(L"\n");
#endif
    }
};

// Global instance
inline CErrorReport g_ErrorReport;
