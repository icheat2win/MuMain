@echo off
REM Build script for MU Online Client on Windows
REM This script automates the CMake configuration and build process

setlocal enabledelayedexpansion

echo ================================================
echo MU Online Client - Windows Build Script
echo ================================================
echo.

REM Check if running in Visual Studio Developer Command Prompt
where cl.exe >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: cl.exe not found in PATH
    echo Please run this script from:
    echo   "Developer Command Prompt for VS 2022"
    echo   OR
    echo   "Developer PowerShell for VS 2022"
    echo.
    pause
    exit /b 1
)

REM Check if cmake is available
where cmake.exe >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: cmake.exe not found in PATH
    echo Please install CMake from https://cmake.org/download/
    echo.
    pause
    exit /b 1
)

REM Check if dotnet is available
where dotnet.exe >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: dotnet.exe not found in PATH
    echo Please install .NET 9 SDK from https://dotnet.microsoft.com/download/dotnet/9.0
    echo.
    pause
    exit /b 1
)

REM Verify .NET version
dotnet --version | findstr /r "^9\." >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo WARNING: .NET 9 SDK not detected
    echo Current version:
    dotnet --version
    echo Required: 9.0 or later
    echo.
)

REM Configuration options
set BUILD_DIR=build
set ARCHITECTURE=Win32
set CONFIGURATION=Release
set BOOST_PATH=C:\Libraries\boost_1_75_0

REM Parse command line arguments
:parse_args
if "%~1"=="" goto end_parse
if /i "%~1"=="--arch" (
    set ARCHITECTURE=%~2
    shift
    shift
    goto parse_args
)
if /i "%~1"=="--config" (
    set CONFIGURATION=%~2
    shift
    shift
    goto parse_args
)
if /i "%~1"=="--boost" (
    set BOOST_PATH=%~2
    shift
    shift
    goto parse_args
)
if /i "%~1"=="--clean" (
    echo Cleaning build directory...
    if exist %BUILD_DIR% rmdir /s /q %BUILD_DIR%
    shift
    goto parse_args
)
if /i "%~1"=="--help" (
    echo Usage: build-windows.bat [options]
    echo.
    echo Options:
    echo   --arch ARCH      Architecture: Win32 or x64 (default: Win32)
    echo   --config CONFIG  Build configuration: Debug or Release (default: Release)
    echo   --boost PATH     Path to Boost installation (default: C:\Libraries\boost_1_75_0)
    echo   --clean          Clean build directory before building
    echo   --help           Show this help message
    echo.
    echo Examples:
    echo   build-windows.bat --arch x64 --config Debug
    echo   build-windows.bat --clean --config Release
    exit /b 0
)
shift
goto parse_args
:end_parse

echo Configuration:
echo   Architecture: %ARCHITECTURE%
echo   Build Type:   %CONFIGURATION%
echo   Boost Path:   %BOOST_PATH%
echo   Build Dir:    %BUILD_DIR%
echo.

REM Check if Boost exists
if not exist "%BOOST_PATH%\boost\version.hpp" (
    echo WARNING: Boost not found at %BOOST_PATH%
    echo Build may fail. Install Boost or use --boost to specify path.
    echo.
)

REM Create build directory
if not exist %BUILD_DIR% (
    echo Creating build directory...
    mkdir %BUILD_DIR%
)

cd %BUILD_DIR%

echo.
echo ================================================
echo Step 1: Configuring with CMake...
echo ================================================
cmake .. -G "Visual Studio 17 2022" -A %ARCHITECTURE% -DBOOST_ROOT="%BOOST_PATH%"
if %ERRORLEVEL% NEQ 0 (
    echo.
    echo ERROR: CMake configuration failed
    cd ..
    pause
    exit /b 1
)

echo.
echo ================================================
echo Step 2: Building C# Library (Native AOT)...
echo ================================================
cmake --build . --config %CONFIGURATION% --target ClientLibrary
if %ERRORLEVEL% NEQ 0 (
    echo.
    echo ERROR: C# library build failed
    cd ..
    pause
    exit /b 1
)

echo.
echo ================================================
echo Step 3: Building C++ Main Executable...
echo ================================================
cmake --build . --config %CONFIGURATION% --target main
if %ERRORLEVEL% NEQ 0 (
    echo.
    echo ERROR: C++ build failed
    cd ..
    pause
    exit /b 1
)

cd ..

echo.
echo ================================================
echo Build Successful!
echo ================================================
echo.
echo Output location: %BUILD_DIR%\bin\%CONFIGURATION%\
echo Executable: main.exe
echo.
echo To run the client:
echo   cd %BUILD_DIR%\bin\%CONFIGURATION%
echo   main.exe
echo.
echo Or with server connection:
echo   main.exe /u192.168.0.20 /p55902
echo.

pause
