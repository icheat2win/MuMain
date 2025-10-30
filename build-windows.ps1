# Build script for MU Online Client on Windows (PowerShell version)
# This script automates the CMake configuration and build process

param(
    [Parameter(HelpMessage="Architecture: Win32 or x64")]
    [ValidateSet("Win32", "x64")]
    [string]$Architecture = "Win32",

    [Parameter(HelpMessage="Build configuration: Debug or Release")]
    [ValidateSet("Debug", "Release")]
    [string]$Configuration = "Release",

    [Parameter(HelpMessage="Path to Boost installation")]
    [string]$BoostPath = "C:\Libraries\boost_1_75_0",

    [Parameter(HelpMessage="Clean build directory before building")]
    [switch]$Clean,

    [Parameter(HelpMessage="Show help")]
    [switch]$Help
)

function Write-Header {
    param([string]$Text)
    Write-Host ""
    Write-Host "================================================" -ForegroundColor Cyan
    Write-Host $Text -ForegroundColor Cyan
    Write-Host "================================================" -ForegroundColor Cyan
}

function Write-Success {
    param([string]$Text)
    Write-Host $Text -ForegroundColor Green
}

function Write-Error {
    param([string]$Text)
    Write-Host $Text -ForegroundColor Red
}

function Write-Warning {
    param([string]$Text)
    Write-Host $Text -ForegroundColor Yellow
}

# Show help if requested
if ($Help) {
    Write-Host "MU Online Client - Windows Build Script (PowerShell)"
    Write-Host ""
    Write-Host "Usage: .\build-windows.ps1 [options]"
    Write-Host ""
    Write-Host "Options:"
    Write-Host "  -Architecture <ARCH>   Architecture: Win32 or x64 (default: Win32)"
    Write-Host "  -Configuration <CFG>   Build configuration: Debug or Release (default: Release)"
    Write-Host "  -BoostPath <PATH>      Path to Boost installation (default: C:\Libraries\boost_1_75_0)"
    Write-Host "  -Clean                 Clean build directory before building"
    Write-Host "  -Help                  Show this help message"
    Write-Host ""
    Write-Host "Examples:"
    Write-Host "  .\build-windows.ps1 -Architecture x64 -Configuration Debug"
    Write-Host "  .\build-windows.ps1 -Clean -Configuration Release"
    exit 0
}

Write-Header "MU Online Client - Windows Build Script"

# Check for required tools
Write-Host "Checking prerequisites..." -ForegroundColor Yellow

# Check for Visual Studio (cl.exe)
$clPath = Get-Command cl.exe -ErrorAction SilentlyContinue
if (-not $clPath) {
    Write-Error "ERROR: cl.exe not found in PATH"
    Write-Host "Please run this script from:"
    Write-Host "  'Developer Command Prompt for VS 2022'"
    Write-Host "  OR"
    Write-Host "  'Developer PowerShell for VS 2022'"
    Write-Host ""
    Write-Host "Press any key to exit..."
    $null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")
    exit 1
}

# Check for CMake
$cmakePath = Get-Command cmake -ErrorAction SilentlyContinue
if (-not $cmakePath) {
    Write-Error "ERROR: cmake not found in PATH"
    Write-Host "Please install CMake from https://cmake.org/download/"
    Write-Host ""
    Write-Host "Press any key to exit..."
    $null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")
    exit 1
}

# Check for dotnet
$dotnetPath = Get-Command dotnet -ErrorAction SilentlyContinue
if (-not $dotnetPath) {
    Write-Error "ERROR: dotnet not found in PATH"
    Write-Host "Please install .NET 9 SDK from https://dotnet.microsoft.com/download/dotnet/9.0"
    Write-Host ""
    Write-Host "Press any key to exit..."
    $null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")
    exit 1
}

# Verify .NET version
$dotnetVersion = & dotnet --version
if (-not ($dotnetVersion -match "^9\.")) {
    Write-Warning "WARNING: .NET 9 SDK not detected"
    Write-Host "Current version: $dotnetVersion"
    Write-Host "Required: 9.0 or later"
    Write-Host ""
}

Write-Success "All prerequisites found!"

# Configuration
$BuildDir = "build"

Write-Host ""
Write-Host "Configuration:" -ForegroundColor Yellow
Write-Host "  Architecture: $Architecture"
Write-Host "  Build Type:   $Configuration"
Write-Host "  Boost Path:   $BoostPath"
Write-Host "  Build Dir:    $BuildDir"
Write-Host ""

# Check Boost
if (-not (Test-Path "$BoostPath\boost\version.hpp")) {
    Write-Warning "WARNING: Boost not found at $BoostPath"
    Write-Host "Build may fail. Install Boost or use -BoostPath to specify correct path."
    Write-Host ""
}

# Clean if requested
if ($Clean -and (Test-Path $BuildDir)) {
    Write-Host "Cleaning build directory..." -ForegroundColor Yellow
    Remove-Item -Recurse -Force $BuildDir
}

# Create build directory
if (-not (Test-Path $BuildDir)) {
    Write-Host "Creating build directory..." -ForegroundColor Yellow
    New-Item -ItemType Directory -Path $BuildDir | Out-Null
}

Set-Location $BuildDir

# Step 1: Configure with CMake
Write-Header "Step 1: Configuring with CMake..."
& cmake .. -G "Visual Studio 17 2022" -A $Architecture -DBOOST_ROOT="$BoostPath"
if ($LASTEXITCODE -ne 0) {
    Write-Error "ERROR: CMake configuration failed"
    Set-Location ..
    Write-Host "Press any key to exit..."
    $null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")
    exit 1
}
Write-Success "Configuration successful!"

# Step 2: Build C# Library
Write-Header "Step 2: Building C# Library (Native AOT)..."
& cmake --build . --config $Configuration --target ClientLibrary
if ($LASTEXITCODE -ne 0) {
    Write-Error "ERROR: C# library build failed"
    Set-Location ..
    Write-Host "Press any key to exit..."
    $null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")
    exit 1
}
Write-Success "C# library built successfully!"

# Step 3: Build C++ Executable
Write-Header "Step 3: Building C++ Main Executable..."
& cmake --build . --config $Configuration --target main
if ($LASTEXITCODE -ne 0) {
    Write-Error "ERROR: C++ build failed"
    Set-Location ..
    Write-Host "Press any key to exit..."
    $null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")
    exit 1
}
Write-Success "C++ executable built successfully!"

Set-Location ..

Write-Header "Build Successful!"
Write-Host ""
Write-Success "Output location: $BuildDir\bin\$Configuration\"
Write-Success "Executable: main.exe"
Write-Host ""
Write-Host "To run the client:"
Write-Host "  cd $BuildDir\bin\$Configuration"
Write-Host "  .\main.exe"
Write-Host ""
Write-Host "Or with server connection:"
Write-Host "  .\main.exe /u192.168.0.20 /p55902"
Write-Host ""
Write-Host "Press any key to exit..."
$null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")
