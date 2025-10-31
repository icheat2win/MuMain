// WindowsTypes.h - Minimal Win32 type shims for non-Windows builds
// Provides enough definitions for the legacy code to compile on macOS/Linux.

#pragma once

#include "Platform/Platform.h"
#include "Platform/PlatformInput.h"
#include "Platform/PlatformWindow.h"

#if !PLATFORM_WINDOWS

#include <algorithm>
#include <chrono>
#include <cerrno>
#include <filesystem>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <cwchar>
#include <cwctype>
#include <locale>
#include <string>
#include <vector>
#include <thread>
#include <type_traits>
#include <codecvt>
#include <cstdarg>

using std::max;
using std::min;

#ifndef CONST
#define CONST const
#endif

typedef int BOOL;
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int UINT;
typedef std::uint32_t DWORD;
typedef std::uint64_t QWORD;
typedef long LONG;
typedef std::uint32_t ULONG;
typedef std::intptr_t LONG_PTR;
typedef std::uintptr_t ULONG_PTR;
typedef std::uintptr_t UINT_PTR;
typedef std::intptr_t INT_PTR;
typedef int INT;
typedef short SHORT;
typedef std::uintptr_t WPARAM;
typedef std::intptr_t LPARAM;
typedef std::intptr_t LRESULT;
typedef void* HANDLE;
typedef void* HINSTANCE;
typedef void* HWND;
typedef void* HDC;
typedef void* HGLRC;
typedef void* HFONT;
typedef void* HMODULE;
typedef void* HBRUSH;
typedef void* HBITMAP;
typedef const char* LPCSTR;
typedef const wchar_t* LPCWSTR;
typedef char* LPSTR;
typedef wchar_t* LPWSTR;
typedef void* LPVOID;
typedef std::uintptr_t DWORD_PTR;
typedef void* PVOID;
typedef unsigned short USHORT;
typedef std::uint64_t ULONGLONG;
typedef std::uint64_t DWORDLONG;
typedef void VOID;
typedef long long __int64;
typedef unsigned long long __uint64;
typedef long HRESULT;
typedef float FLOAT;
typedef void* HHOOK;

#ifndef _MAX_DRIVE
#define _MAX_DRIVE 3
#endif

#ifndef _MAX_DIR
#define _MAX_DIR 256
#endif

#ifndef _MAX_FNAME
#define _MAX_FNAME 256
#endif

#ifndef _MAX_EXT
#define _MAX_EXT 256
#endif

#ifndef STRUNCATE
#define STRUNCATE 80
#endif

#ifndef _TRUNCATE
#define _TRUNCATE ((size_t)-1)
#endif

#ifndef MB_OK
#define MB_OK 0x00000000L
#define MB_OKCANCEL 0x00000001L
#define MB_ABORTRETRYIGNORE 0x00000002L
#define MB_YESNOCANCEL 0x00000003L
#define MB_YESNO 0x00000004L
#define MB_RETRYCANCEL 0x00000005L
#define MB_ICONERROR 0x00000010L
#define MB_ICONQUESTION 0x00000020L
#define MB_ICONWARNING 0x00000030L
#define MB_ICONINFORMATION 0x00000040L
#define MB_ICONEXCLAMATION MB_ICONWARNING
#define MB_ICONSTOP MB_ICONERROR
#endif

#ifndef WM_NULL
#define WM_NULL 0x0000
#define WM_CREATE 0x0001
#define WM_DESTROY 0x0002
#define WM_MOVE 0x0003
#define WM_SIZE 0x0005
#define WM_ACTIVATE 0x0006
#define WM_SETFOCUS 0x0007
#define WM_KILLFOCUS 0x0008
#define WM_ENABLE 0x000A
#define WM_SETREDRAW 0x000B
#define WM_SETTEXT 0x000C
#define WM_GETTEXT 0x000D
#define WM_GETTEXTLENGTH 0x000E
#define WM_PAINT 0x000F
#define WM_CLOSE 0x0010
#define WM_QUERYENDSESSION 0x0011
#define WM_QUIT 0x0012
#define WM_ERASEBKGND 0x0014
#define WM_SYSCOLORCHANGE 0x0015
#define WM_SHOWWINDOW 0x0018
#define WM_SETCURSOR 0x0020
#define WM_MOUSEMOVE 0x0200
#define WM_LBUTTONDOWN 0x0201
#define WM_LBUTTONUP 0x0202
#define WM_LBUTTONDBLCLK 0x0203
#define WM_RBUTTONDOWN 0x0204
#define WM_RBUTTONUP 0x0205
#define WM_MBUTTONDOWN 0x0207
#define WM_MBUTTONUP 0x0208
#define WM_MOUSEWHEEL 0x020A
#define WM_KEYDOWN 0x0100
#define WM_KEYUP 0x0101
#define WM_CHAR 0x0102
#define WM_SYSKEYDOWN 0x0104
#define WM_TIMER 0x0113
#define WM_IME_STARTCOMPOSITION 0x010D
#define WM_IME_ENDCOMPOSITION 0x010E
#define WM_IME_COMPOSITION 0x010F
#define WM_IME_NOTIFY 0x0282
#define WM_IME_CONTROL 0x0283
#define WM_USER 0x0400
#endif

#ifndef EM_SETSEL
#define EM_SETSEL 0x00B1
#define EM_REPLACESEL 0x00C2
#endif

#ifndef IMC_SETCOMPOSITIONWINDOW
#define IMC_SETCOMPOSITIONWINDOW 0x000C
#endif

#ifndef VK_LBUTTON
#define VK_LBUTTON 0x01
#define VK_RBUTTON 0x02
#define VK_CANCEL 0x03
#define VK_MBUTTON 0x04
#define VK_XBUTTON1 0x05
#define VK_XBUTTON2 0x06
#define VK_BACK 0x08
#define VK_TAB 0x09
#define VK_RETURN 0x0D
#define VK_SHIFT 0x10
#define VK_CONTROL 0x11
#define VK_MENU 0x12
#define VK_PAUSE 0x13
#define VK_CAPITAL 0x14
#define VK_ESCAPE 0x1B
#define VK_SPACE 0x20
#define VK_PRIOR 0x21
#define VK_NEXT 0x22
#define VK_END 0x23
#define VK_HOME 0x24
#define VK_LEFT 0x25
#define VK_UP 0x26
#define VK_RIGHT 0x27
#define VK_DOWN 0x28
#define VK_SNAPSHOT 0x2C
#define VK_INSERT 0x2D
#define VK_DELETE 0x2E
#define VK_0 0x30
#define VK_1 0x31
#define VK_2 0x32
#define VK_3 0x33
#define VK_4 0x34
#define VK_5 0x35
#define VK_6 0x36
#define VK_7 0x37
#define VK_8 0x38
#define VK_9 0x39
#define VK_A 0x41
#define VK_B 0x42
#define VK_C 0x43
#define VK_D 0x44
#define VK_E 0x45
#define VK_F 0x46
#define VK_G 0x47
#define VK_H 0x48
#define VK_I 0x49
#define VK_J 0x4A
#define VK_K 0x4B
#define VK_L 0x4C
#define VK_M 0x4D
#define VK_N 0x4E
#define VK_O 0x4F
#define VK_P 0x50
#define VK_Q 0x51
#define VK_R 0x52
#define VK_S 0x53
#define VK_T 0x54
#define VK_U 0x55
#define VK_V 0x56
#define VK_W 0x57
#define VK_X 0x58
#define VK_Y 0x59
#define VK_Z 0x5A
#define VK_NUMPAD0 0x60
#define VK_NUMPAD1 0x61
#define VK_NUMPAD2 0x62
#define VK_NUMPAD3 0x63
#define VK_NUMPAD4 0x64
#define VK_NUMPAD5 0x65
#define VK_NUMPAD6 0x66
#define VK_NUMPAD7 0x67
#define VK_NUMPAD8 0x68
#define VK_NUMPAD9 0x69
#define VK_MULTIPLY 0x6A
#define VK_ADD 0x6B
#define VK_SEPARATOR 0x6C
#define VK_SUBTRACT 0x6D
#define VK_DECIMAL 0x6E
#define VK_DIVIDE 0x6F
#define VK_F1 0x70
#define VK_F2 0x71
#define VK_F3 0x72
#define VK_F4 0x73
#define VK_F5 0x74
#define VK_F6 0x75
#define VK_F7 0x76
#define VK_F8 0x77
#define VK_F9 0x78
#define VK_F10 0x79
#define VK_F11 0x7A
#define VK_F12 0x7B
#define VK_NUMLOCK 0x90
#define VK_SCROLL 0x91
#define VK_LSHIFT 0xA0
#define VK_RSHIFT 0xA1
#define VK_LCONTROL 0xA2
#define VK_RCONTROL 0xA3
#define VK_LMENU 0xA4
#define VK_RMENU 0xA5
#define VK_OEM_PLUS 0xBB
#define VK_OEM_COMMA 0xBC
#define VK_OEM_MINUS 0xBD
#define VK_OEM_PERIOD 0xBE
#endif

struct RECT
{
    LONG left;
    LONG top;
    LONG right;
    LONG bottom;
};

struct POINT
{
    LONG x;
    LONG y;
};

struct SIZE
{
    LONG cx;
    LONG cy;
};

inline BOOL PtInRect(const RECT* rect, POINT point)
{
    if (!rect)
    {
        return 0;
    }

    return (point.x >= rect->left && point.x < rect->right &&
            point.y >= rect->top && point.y < rect->bottom) ? 1 : 0;
}

inline BOOL OffsetRect(RECT* rect, int dx, int dy)
{
    if (!rect)
    {
        return 0;
    }

    rect->left += dx;
    rect->right += dx;
    rect->top += dy;
    rect->bottom += dy;
    return 1;
}

inline int _wcsicmp(const wchar_t* lhs, const wchar_t* rhs)
{
    if (!lhs)
    {
        lhs = L"";
    }
    if (!rhs)
    {
        rhs = L"";
    }

#if defined(_GNU_SOURCE) || defined(__APPLE__) || defined(__linux__)
    return wcscasecmp(lhs, rhs);
#else
    std::wstring left(lhs);
    std::wstring right(rhs);
    std::transform(left.begin(), left.end(), left.begin(), ::towlower);
    std::transform(right.begin(), right.end(), right.begin(), ::towlower);
    return left.compare(right);
#endif
}

inline int wcsicmp(const wchar_t* lhs, const wchar_t* rhs)
{
    return _wcsicmp(lhs, rhs);
}

inline wchar_t* _wcstok(wchar_t* str, const wchar_t* delimiters)
{
    static thread_local wchar_t* context = nullptr;
    return wcstok(str, delimiters, &context);
}

inline int wcsnicmp(const wchar_t* lhs, const wchar_t* rhs, size_t count)
{
    if (!lhs)
    {
        lhs = L"";
    }
    if (!rhs)
    {
        rhs = L"";
    }

#if defined(_GNU_SOURCE) || defined(__APPLE__) || defined(__linux__)
    return wcsncasecmp(lhs, rhs, count);
#else
    std::wstring left(lhs, lhs + std::min(count, std::wcslen(lhs)));
    std::wstring right(rhs, rhs + std::min(count, std::wcslen(rhs)));
    std::transform(left.begin(), left.end(), left.begin(), ::towlower);
    std::transform(right.begin(), right.end(), right.begin(), ::towlower);
    return left.compare(right);
#endif
}

inline void wstring_copy_to_buffer(const std::wstring& source, wchar_t* destination, size_t destinationSize)
{
    if (!destination || destinationSize == 0)
    {
        return;
    }

    size_t copyLength = std::min(source.size(), destinationSize - 1);
    if (copyLength > 0)
    {
        std::wmemcpy(destination, source.c_str(), copyLength);
    }
    destination[copyLength] = L'\0';
}

inline void _wsplitpath(const wchar_t* path,
                        wchar_t* drive,
                        wchar_t* dir,
                        wchar_t* fname,
                        wchar_t* ext)
{
    if (drive)
    {
        drive[0] = L'\0';
    }
    if (dir)
    {
        dir[0] = L'\0';
    }
    if (fname)
    {
        fname[0] = L'\0';
    }
    if (ext)
    {
        ext[0] = L'\0';
    }

    if (!path)
    {
        return;
    }

    std::filesystem::path fsPath(path);

    if (dir)
    {
        const auto parent = fsPath.parent_path().wstring();
        wstring_copy_to_buffer(parent, dir, _MAX_DIR);
    }

    if (fname)
    {
        const auto stem = fsPath.stem().wstring();
        wstring_copy_to_buffer(stem, fname, _MAX_FNAME);
    }

    if (ext)
    {
        const auto extension = fsPath.extension().wstring();
        wstring_copy_to_buffer(extension, ext, _MAX_EXT);
    }
}

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

#ifndef CALLBACK
#define CALLBACK
#endif

#ifndef WINAPI
#define WINAPI
#endif

#ifndef APIENTRY
#define APIENTRY
#endif

#ifndef MAX_PATH
#define MAX_PATH 260
#endif

#ifndef TCHAR
typedef wchar_t TCHAR;
#endif

#ifndef LPTSTR
typedef TCHAR* LPTSTR;
#endif

#ifndef LPCTSTR
typedef const TCHAR* LPCTSTR;
#endif

#ifndef LPSIZE
typedef SIZE* LPSIZE;
#endif

#ifndef _T
#define _T(x) L##x
#endif

#ifndef TEXT
#define TEXT(x) L##x
#endif

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef OPTIONAL
#define OPTIONAL
#endif

#ifndef LOWORD
#define LOWORD(l)   (static_cast<WORD>((static_cast<DWORD_PTR>(l)) & 0xffff))
#endif

#ifndef HIWORD
#define HIWORD(l)   (static_cast<WORD>((static_cast<DWORD_PTR>(l) >> 16) & 0xffff))
#endif

#ifndef LOBYTE
#define LOBYTE(w)   (static_cast<BYTE>((static_cast<DWORD>(w)) & 0xff))
#endif

#ifndef HIBYTE
#define HIBYTE(w)   (static_cast<BYTE>((static_cast<DWORD>(w) >> 8) & 0xff))
#endif

#ifndef MAKELONG
#define MAKELONG(l, h)   ((static_cast<LONG>(static_cast<WORD>(l))) | (static_cast<LONG>(static_cast<WORD>(h)) << 16))
#endif

#ifndef RGB
#define RGB(r, g, b) ((static_cast<DWORD>(static_cast<BYTE>(r))      ) | \
                      (static_cast<DWORD>(static_cast<BYTE>(g)) << 8 ) | \
                      (static_cast<DWORD>(static_cast<BYTE>(b)) << 16))
#endif

#ifndef ZeroMemory
#define ZeroMemory(Destination, Length) std::memset((Destination), 0, (Length))
#endif

#ifndef CopyMemory
#define CopyMemory(Destination, Source, Length) std::memcpy((Destination), (Source), (Length))
#endif

#ifndef MoveMemory
#define MoveMemory(Destination, Source, Length) std::memmove((Destination), (Source), (Length))
#endif

inline void Sleep(unsigned long milliseconds)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

inline DWORD GetTickCount()
{
    using namespace std::chrono;
    return static_cast<DWORD>(duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count());
}

inline unsigned long long GetTickCount64()
{
    using namespace std::chrono;
    return static_cast<unsigned long long>(duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count());
}

inline void OutputDebugStringA(const char* message)
{
    if (message)
    {
        std::fprintf(stderr, "%s", message);
    }
}

inline void OutputDebugStringW(const wchar_t* message)
{
    if (message)
    {
        std::fwprintf(stderr, L"%ls", message);
    }
}

inline int MessageBoxA(HWND, const char* text, const char* caption, unsigned int)
{
    std::fprintf(stderr, "MessageBoxA: %s - %s\n",
                 caption ? caption : "(no caption)",
                 text ? text : "(no text)");
    return 0;
}

inline int MessageBoxW(HWND, const wchar_t* text, const wchar_t* caption, unsigned int)
{
    std::fwprintf(stderr, L"MessageBoxW: %ls - %ls\n",
                  caption ? caption : L"(no caption)",
                  text ? text : L"(no text)");
    return 0;
}

inline int MessageBox(HWND hwnd, const wchar_t* text, const wchar_t* caption, unsigned int flags)
{
    return MessageBoxW(hwnd, text, caption, flags);
}

inline int MessageBox(HWND hwnd, const char* text, const char* caption, unsigned int flags)
{
    return MessageBoxA(hwnd, text, caption, flags);
}

inline LRESULT PostMessage(HWND, unsigned int msg, WPARAM, LPARAM)
{
    auto* window = Platform::WindowManager::GetMainWindow();
    if (!window)
    {
        return 0;
    }

    switch (msg)
    {
    case WM_CLOSE:
    case WM_DESTROY:
        window->SetShouldClose(true);
        break;
    default:
        break;
    }

    return 0;
}

inline LRESULT SendMessage(HWND hwnd, unsigned int msg, WPARAM wParam, LPARAM lParam)
{
    return PostMessage(hwnd, msg, wParam, lParam);
}

inline BOOL DestroyWindow(HWND)
{
    return TRUE;
}

inline UINT GetDoubleClickTime()
{
    return 500;
}

inline SHORT GetAsyncKeyState(int vKey)
{
    auto key = static_cast<Platform::KeyCode>(vKey);
    return Platform::Input::IsKeyDown(key) ? static_cast<SHORT>(0x8000) : 0;
}

inline SHORT GetKeyState(int vKey)
{
    auto key = static_cast<Platform::KeyCode>(vKey);
    return Platform::Input::IsKeyDown(key) ? static_cast<SHORT>(0x8000) : 0;
}

inline BOOL GetCursorPos(POINT* point)
{
    if (!point)
    {
        return FALSE;
    }

    int x = 0;
    int y = 0;
    Platform::Input::GetMousePosition(x, y);
    point->x = x;
    point->y = y;
    return TRUE;
}

inline BOOL ScreenToClient(HWND, POINT*)
{
    return TRUE;
}

inline HWND GetActiveWindow()
{
    auto* window = Platform::WindowManager::GetMainWindow();
    if (window && window->IsFocused())
    {
        return reinterpret_cast<HWND>(window->GetHandle());
    }
    return nullptr;
}

inline HWND GetFocus()
{
    auto* window = Platform::WindowManager::GetMainWindow();
    if (window && window->IsFocused())
    {
        return reinterpret_cast<HWND>(window->GetHandle());
    }
    return nullptr;
}

inline HWND SetFocus(HWND hwnd)
{
    auto* window = Platform::WindowManager::GetMainWindow();
    if (window)
    {
        return reinterpret_cast<HWND>(window->GetHandle());
    }
    return hwnd;
}

inline BOOL GetTextExtentPoint32A(HDC, const char* text, int count, SIZE* size)
{
    if (!text || !size)
    {
        return FALSE;
    }

    int length = (count >= 0) ? count : static_cast<int>(std::strlen(text));
    if (length < 0)
    {
        length = 0;
    }

    size->cx = length * 8;
    size->cy = 16;
    return TRUE;
}

inline BOOL GetTextExtentPoint32W(HDC, const wchar_t* text, int count, SIZE* size)
{
    if (!text || !size)
    {
        return FALSE;
    }

    int length = (count >= 0) ? count : static_cast<int>(std::wcslen(text));
    if (length < 0)
    {
        length = 0;
    }

    size->cx = length * 8;
    size->cy = 16;
    return TRUE;
}

#ifndef GetTextExtentPoint32
#define GetTextExtentPoint32 GetTextExtentPoint32W
#endif

typedef DWORD* LPDWORD;
typedef BOOL* LPBOOL;
typedef WORD* LPWORD;
typedef BYTE* LPBYTE;
typedef ULONG* LPULONG;
typedef LONG* LPLONG;
typedef void* LPSECURITY_ATTRIBUTES;
typedef BYTE* PBYTE;

#ifndef WNDPROC
typedef LRESULT (*WNDPROC)(HWND, unsigned int, WPARAM, LPARAM);
#endif

struct OVERLAPPED
{
    ULONG_PTR Internal;
    ULONG_PTR InternalHigh;
    union
    {
        struct
        {
            DWORD Offset;
            DWORD OffsetHigh;
        };
        PVOID Pointer;
    };
    HANDLE hEvent;
};

typedef OVERLAPPED* LPOVERLAPPED;

#ifndef FOREGROUND_BLUE
#define FOREGROUND_BLUE      0x0001
#endif

#ifndef FOREGROUND_GREEN
#define FOREGROUND_GREEN     0x0002
#endif

#ifndef FOREGROUND_RED
#define FOREGROUND_RED       0x0004
#endif

#ifndef FOREGROUND_INTENSITY
#define FOREGROUND_INTENSITY 0x0008
#endif

#ifndef BACKGROUND_BLUE
#define BACKGROUND_BLUE      0x0010
#endif

#ifndef BACKGROUND_GREEN
#define BACKGROUND_GREEN     0x0020
#endif

#ifndef BACKGROUND_RED
#define BACKGROUND_RED       0x0040
#endif

#ifndef BACKGROUND_INTENSITY
#define BACKGROUND_INTENSITY 0x0080
#endif

#ifndef S_OK
#define S_OK ((HRESULT)0L)
#endif

#ifndef FAILED
#define FAILED(hr) (((HRESULT)(hr)) < 0)
#endif

#ifndef DEFINE_ENUM_FLAG_OPERATORS
#define DEFINE_ENUM_FLAG_OPERATORS(ENUMTYPE) \
inline ENUMTYPE operator|(ENUMTYPE a, ENUMTYPE b) { \
    using Underlying = std::underlying_type_t<ENUMTYPE>; \
    return static_cast<ENUMTYPE>(static_cast<Underlying>(a) | static_cast<Underlying>(b)); \
} \
inline ENUMTYPE operator&(ENUMTYPE a, ENUMTYPE b) { \
    using Underlying = std::underlying_type_t<ENUMTYPE>; \
    return static_cast<ENUMTYPE>(static_cast<Underlying>(a) & static_cast<Underlying>(b)); \
} \
inline ENUMTYPE operator^(ENUMTYPE a, ENUMTYPE b) { \
    using Underlying = std::underlying_type_t<ENUMTYPE>; \
    return static_cast<ENUMTYPE>(static_cast<Underlying>(a) ^ static_cast<Underlying>(b)); \
} \
inline ENUMTYPE& operator|=(ENUMTYPE& a, ENUMTYPE b) { \
    a = a | b; \
    return a; \
} \
inline ENUMTYPE& operator&=(ENUMTYPE& a, ENUMTYPE b) { \
    a = a & b; \
    return a; \
} \
inline ENUMTYPE& operator^=(ENUMTYPE& a, ENUMTYPE b) { \
    a = a ^ b; \
    return a; \
} \
inline ENUMTYPE operator~(ENUMTYPE a) { \
    using Underlying = std::underlying_type_t<ENUMTYPE>; \
    return static_cast<ENUMTYPE>(~static_cast<Underlying>(a)); \
}
#endif

#ifndef CP_UTF8
#define CP_UTF8 65001
#endif

inline int MultiByteToWideChar(unsigned int /*codePage*/, DWORD /*flags*/, const char* multiByteStr, int multiByteLen, wchar_t* wideCharStr, int wideCharLen)
{
    if (!multiByteStr)
    {
        return 0;
    }

    std::string input;
    if (multiByteLen < 0)
    {
        input = std::string(multiByteStr);
    }
    else
    {
        input.assign(multiByteStr, static_cast<std::size_t>(multiByteLen));
    }

    try
    {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        std::wstring wide = converter.from_bytes(input);

        if (!wideCharStr || wideCharLen <= 0)
        {
            return static_cast<int>(wide.size() + 1);
        }

        if (static_cast<int>(wide.size()) >= wideCharLen)
        {
            return 0;
        }

        std::wcsncpy(wideCharStr, wide.c_str(), static_cast<std::size_t>(wideCharLen));
        wideCharStr[wide.size()] = L'\0';
        return static_cast<int>(wide.size());
    }
    catch (...)
    {
        return 0;
    }
}

inline int WideCharToMultiByte(unsigned int /*codePage*/, DWORD /*flags*/, const wchar_t* wideCharStr, int wideCharLen, char* multiByteStr, int multiByteLen, const char*, BOOL*)
{
    if (!wideCharStr)
    {
        return 0;
    }

    std::wstring input;
    if (wideCharLen < 0)
    {
        input = std::wstring(wideCharStr);
    }
    else
    {
        input.assign(wideCharStr, static_cast<std::size_t>(wideCharLen));
    }

    try
    {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        std::string narrow = converter.to_bytes(input);

        if (!multiByteStr || multiByteLen <= 0)
        {
            return static_cast<int>(narrow.size() + 1);
        }

        if (static_cast<int>(narrow.size()) >= multiByteLen)
        {
            return 0;
        }

        std::strncpy(multiByteStr, narrow.c_str(), static_cast<std::size_t>(multiByteLen));
        multiByteStr[narrow.size()] = '\0';
        return static_cast<int>(narrow.size());
    }
    catch (...)
    {
        return 0;
    }
}

inline wchar_t* _itow(int value, wchar_t* buffer, int radix)
{
    if (!buffer)
    {
        return nullptr;
    }

    std::wstring result;

    switch (radix)
    {
        case 10:
            result = std::to_wstring(value);
            break;
        case 16:
        {
            wchar_t temp[32];
            std::swprintf(temp, sizeof(temp) / sizeof(temp[0]), L"%x", value);
            result = temp;
            break;
        }
        default:
            result = std::to_wstring(value);
            break;
    }

    std::wcsncpy(buffer, result.c_str(), result.size());
    buffer[result.size()] = L'\0';

    return buffer;
}

inline int _wtoi(const wchar_t* str)
{
    if (!str)
    {
        return 0;
    }

    return static_cast<int>(std::wcstol(str, nullptr, 10));
}

inline FILE* _wfopen(const wchar_t* filename, const wchar_t* mode)
{
    if (!filename || !mode)
    {
        return nullptr;
    }

    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::string narrowName = converter.to_bytes(filename);
    std::string narrowMode = converter.to_bytes(mode);
    return std::fopen(narrowName.c_str(), narrowMode.c_str());
}

inline DWORD timeGetTime()
{
    return GetTickCount();
}

inline int wcscpy_s(wchar_t* destination, size_t destinationSize, const wchar_t* source)
{
    if (!destination || destinationSize == 0)
    {
        return EINVAL;
    }

    if (!source)
    {
        destination[0] = L'\0';
        return EINVAL;
    }

    const size_t sourceLength = std::wcslen(source);
    if (sourceLength >= destinationSize)
    {
        std::wcsncpy(destination, source, destinationSize - 1);
        destination[destinationSize - 1] = L'\0';
        return ERANGE;
    }

    std::wcscpy(destination, source);
    return 0;
}

inline int wcsncpy_s(wchar_t* destination, size_t destinationSize, const wchar_t* source, size_t count)
{
    if (!destination || destinationSize == 0)
    {
        return EINVAL;
    }

    destination[0] = L'\0';

    if (!source)
    {
        return EINVAL;
    }

    const size_t maxCopy = destinationSize > 0 ? destinationSize - 1 : 0;
    size_t copyLength = 0;

    if (count == _TRUNCATE)
    {
        const size_t sourceLength = std::wcslen(source);
        copyLength = std::min(sourceLength, maxCopy);
        std::wmemcpy(destination, source, copyLength);
        destination[copyLength] = L'\0';
        return (sourceLength > maxCopy) ? STRUNCATE : 0;
    }

    copyLength = std::min(count, maxCopy);
    std::wmemcpy(destination, source, copyLength);
    destination[copyLength] = L'\0';

    if (count > maxCopy && std::wcslen(source) > maxCopy)
    {
        return ERANGE;
    }

    return 0;
}


namespace Platform { namespace Detail {

template <size_t N>
int WideSnprintf(wchar_t (&buffer)[N], const wchar_t* format, ...)
{
    va_list args;
    va_start(args, format);
    int result = vswprintf(buffer, N, format, args);
    va_end(args);
    return result;
}

inline int WideSnprintf(wchar_t* buffer, const wchar_t* format, ...)
{
    if (!buffer || !format)
    {
        return -1;
    }

    va_list args;
    va_start(args, format);
    va_list argsCopy;
    va_copy(argsCopy, args);

    int required = vswprintf(nullptr, 0, format, argsCopy);
    va_end(argsCopy);
    if (required < 0)
    {
        va_end(args);
        return required;
    }

    std::vector<wchar_t> temp(static_cast<size_t>(required) + 1);
    int written = vswprintf(temp.data(), temp.size(), format, args);
    va_end(args);

    if (written >= 0)
    {
        std::wmemcpy(buffer, temp.data(), static_cast<size_t>(written) + 1);
    }

    return written;
}

} }

#ifndef swprintf
#define swprintf(buffer, ...) Platform::Detail::WideSnprintf(buffer, __VA_ARGS__)
#endif

#endif // !PLATFORM_WINDOWS
