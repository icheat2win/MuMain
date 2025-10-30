# MU Online Client Sources

This is my special fork of the Season 5.2 client sources [uploaded by Luois](https://github.com/LouisEmulator/Main5.2).

The ultimate goal is to clean it up and make it compatible and feature complete
to Season 6 Episode 3.

## 🎉 Now Cross-Platform!

This fork includes a complete **cross-platform build system** supporting:
- ✅ **Windows 11** (x86/x64)
- ✅ **Linux** (x86_64/ARM64)
- ✅ **macOS** (Intel/Apple Silicon)

**NEW:** GLFW window abstraction implemented and ready to test!

### Documentation
- **[QUICKSTART.md](QUICKSTART.md)** - Get started in 5 minutes
- **[BUILD.md](BUILD.md)** - Detailed build instructions
- **[CROSSPLATFORM.md](CROSSPLATFORM.md)** - Architecture overview
- **[MIGRATION.md](MIGRATION.md)** - How to migrate Windows code
- **[GLFW_IMPLEMENTATION.md](GLFW_IMPLEMENTATION.md)** - GLFW window system (NEW!)

What I have done so far:
  * 🔥 The framerate has been increased.
    * By default, it uses V-Sync without fps limit. If V-Sync is not
    available, it limits to 60 fps.
    * The options menu includes a checkbox to reduce effects to achieve higher frame rates.
    * Chat commands:
      * Change FPS-Limit: `$fps <value>`
      * Enable V-Sync: `$vsync on`
      * Disable V-Sync: `$vsync off`
  * 🔥 Optimized some OpenGL calls by using vertex arrays. This should result in
    a better frame rate when many players and objects are visible.
  * 🔥 Added inventory and vault extensions.
  * 🔥 The master skill tree system was upgraded to Season 6
  * 🔥 Unicode support: The client works with UTF-16LE instead of ANSI in memory.
    All strings and char arrays have been changed to use wide characters.
    Strings coming from files and the network are handled as UTF-8.
  * 🔥 Replaced the network stack with MUnique.OpenMU.Network to make it easier to
    apply changes. This repository includes a C# .NET 9 client library which is built
    with Native AOT.
  * 🔥 The network protocol has been adapted for Season 6 Episode 3 - there is probably
    still some work to do, but it connects to [OpenMU](https://github.com/MUnique/OpenMU)
    and is playable. Additionally, the protocol has been extended so it's not standard
    anymore.
    * Damage, Exp etc. can exceed 16 bit now.
    * Improved item serialization
    * Improved appearance serialization
    * Added monster health status bar after attack
  * 🔥 Significant changes from Qubit have been incorporated, such as
    * Rage Fighter class
    * Visual bug when Dark Lord walks with Raven
    * Item equipping with right mouse click
    * Glow for red, blue and black fenrir
    * Additional screen resolutions
  * 🔥 Incorporated MU Helper UI and logic - there's some work to do but core functionality is usable
  * Removed if-defs for Rage Fighter class as we are targeting Season 6, so Rage
    Fighter should always be included.
  * Some minor bug fixes, e.g.:
    * Storm Crow item labels
    * Ancient set labels
  * The code has been refactored. A lot of magic values have been replaced by
    enums and constants.

What needs to be done for Season 6:
  * Lucky Items

## How to build & run

### Quick Start (Windows)

**New: CMake Build System** 🎉

The project now includes a cross-platform CMake build system that simplifies building:

```cmd
# Using the automated build script (recommended)
build-windows.bat

# Or using PowerShell
.\build-windows.ps1
```

See **[BUILD.md](BUILD.md)** for detailed build instructions for Windows, Linux, and macOS.

### Requirements

  * **Visual Studio 2022** with the newest update, workloads for C++ and C#
  * **.NET 9 SDK** with Native AOT support
  * **CMake 3.25+** (for new build system)
  * **Boost 1.75.0** (install at `C:\Libraries\boost_1_75_0` or specify path)
  * A compatible server: [OpenMU](https://github.com/MUnique/OpenMU)

### Building

**Option 1: Automated Build (Easiest)**

Run from Developer Command Prompt for VS 2022:

```cmd
build-windows.bat --config Release
```

**Option 2: Manual CMake Build**

```cmd
mkdir build && cd build
cmake .. -G "Visual Studio 17 2022" -A Win32
cmake --build . --config Release
```

**Option 3: Original Visual Studio Solution**

The original `Main.sln` is still available in `Source Main 5.2/`. Note: You must manually publish the C# library first:

```cmd
cd ClientLibrary
dotnet publish -c Release -r win-x86 --self-contained -p:PublishAot=true
```

### Running

The CMake build automatically handles the C# library compilation. After building:

```cmd
cd build/bin/Release
main.exe
```

It supports the common starting parameters `/u` and `/p`, example: `main.exe /u192.168.0.20 /p55902`.
The [OpenMU launcher](https://github.com/MUnique/OpenMU/releases/download/v0.8.17/MUnique.OpenMU.ClientLauncher_0.8.17.zip)
will work as well. By default, it connects to localhost and port `44406`.
The client identifies itself with Version `2.04d` and serial `k1Pk2jcET48mxL3b`.

## Credits

  * Webzen
  * Louis
  * Qubit (tuservermu.com.ve)
  * Community members of RaGEZONE and tuservermu.com.ve for posting fixes
  * [Nitoy](https://github.com/nitoygo) for the MU Helper