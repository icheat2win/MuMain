#pragma once

// Stub implementation for Windows console
class CWindowsConsole
{
public:
    CWindowsConsole() {}
    ~CWindowsConsole() {}

    void Create() {}
    void Destroy() {}
    void Write(const wchar_t* format, ...) {}
};

// Global instance  
inline CWindowsConsole g_WindowsConsole;
