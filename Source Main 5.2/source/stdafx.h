// stdafx.h : include file for standard system include files,
#pragma once

#include "Platform/Platform.h"
#include "Platform/WindowsTypes.h"

#if defined(_MSC_VER)
// MSVC-specific warning adjustments
#pragma warning(disable : 4067)
#pragma warning(disable : 4786)
#pragma warning(disable : 4800)
#pragma warning(disable : 4996)
#pragma warning(disable : 4244)
#pragma warning(disable : 4237)
#pragma warning(disable : 4305)
#pragma warning(disable : 4503)
#pragma warning(disable : 4267)
#pragma warning(disable : 4091)
#pragma warning(disable : 4819)
#pragma warning(disable : 4505)
#pragma warning(disable : 4100)
#pragma warning(disable : 4127)
#pragma warning(disable : 4702)
#pragma warning(disable : 4838)
#pragma warning(disable : 5208)
#pragma warning(disable : 28159)
#pragma warning(disable : 26812)
#endif // _MSC_VER

#if PLATFORM_WINDOWS
// Exclude rarely-used stuff from Windows headers
#define WIN32_LEAN_AND_MEAN

#ifndef _USE_32BIT_TIME_T
#define _USE_32BIT_TIME_T
#endif //_USE_32BIT_TIME_T

#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_NONSTDC_NO_DEPRECATE

#if defined(_MSC_VER)
#pragma warning(push, 3)
#endif

#include <windows.h>
#include <WinSock2.h>
#include <mmsystem.h>
#include <shellapi.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <mbstring.h>
#include <conio.h>

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#else // PLATFORM_WINDOWS

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#endif // PLATFORM_WINDOWS

// C runtime
#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include <cassert>
#include <cstring>
#include <ctime>
#include <cmath>

// STL containers
#include <string>
#include <list>
#include <map>
#include <deque>
#include <algorithm>
#include <vector>
#include <queue>

// OpenGL
#include <GL/glew.h>
#include <GL/gl.h>

// Patch
// winmain
#include "Winmain.h"
#include "Defined_Global.h"

// client
#include "_define.h"
#include "_enum.h"
#include "GameTypes.h"
#include "_struct.h"
#include "w_WindowMessageHandler.h"
#include "_GlobalFunctions.h"
#include "_TextureIndex.h"
#include "UIDefaultBase.h"
#include "NewUICommon.h"
#include "./Math/ZzzMathLib.h"
#include "ZzzOpenglUtil.h"

#include "MultiLanguage.h"

#include "./Utilities/Log/muConsoleDebug.h"
#include "./Utilities/Log/ErrorReport.h"
#include "./Utilities/Log/WindowsConsole.h"

#include "w_MapHeaders.h"

#include "_crypt.h"

inline std::wstring g_strSelectedML = L"";
inline float g_fScreenRate_x = 0;
inline float g_fScreenRate_y = 0;
