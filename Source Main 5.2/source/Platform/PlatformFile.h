// PlatformFile.h - Cross-platform file I/O abstraction
// Replaces Windows CreateFile, ReadFile, WriteFile, etc.

#pragma once

#include <cstdint>
#include <cstdio>

namespace Platform
{
    // File access modes
    enum class FileAccess : uint32_t
    {
        Read = 0x01,
        Write = 0x02,
        ReadWrite = 0x03
    };

    // File creation modes
    enum class FileMode : uint32_t
    {
        OpenExisting,    // Open existing file, fail if doesn't exist
        CreateAlways,    // Create new file, overwrite if exists
        CreateNew,       // Create new file, fail if exists
        OpenAlways,      // Open if exists, create if doesn't
        TruncateExisting // Open and truncate to 0 size
    };

    // File seek origin
    enum class FileSeek : uint32_t
    {
        Begin = 0,
        Current = 1,
        End = 2
    };

    // File attributes
    enum class FileAttributes : uint32_t
    {
        None = 0,
        ReadOnly = 0x01,
        Hidden = 0x02,
        System = 0x04,
        Directory = 0x10,
        Archive = 0x20
    };

    // File handle (platform-specific)
    #if PLATFORM_WINDOWS
        typedef void* FileHandle;  // HANDLE
    #else
        typedef FILE* FileHandle;
    #endif

    constexpr FileHandle INVALID_FILE_HANDLE = nullptr;

    // File class
    class File
    {
    public:
        File();
        ~File();

        // Open file
        bool Open(const char* path, FileAccess access, FileMode mode);

        // Close file
        void Close();

        // Read from file
        size_t Read(void* buffer, size_t size);

        // Write to file
        size_t Write(const void* buffer, size_t size);

        // Seek in file
        bool Seek(int64_t offset, FileSeek origin);

        // Get current position
        int64_t Tell() const;

        // Get file size
        int64_t GetSize() const;

        // Flush buffers
        void Flush();

        // Check if file is open
        bool IsOpen() const { return m_Handle != INVALID_FILE_HANDLE; }

        // Get file handle
        FileHandle GetHandle() const { return m_Handle; }

    private:
        FileHandle m_Handle;
        char m_Path[512];
    };

    // File utility functions
    namespace FileSystem
    {
        // Check if file exists
        bool FileExists(const char* path);

        // Check if directory exists
        bool DirectoryExists(const char* path);

        // Create directory
        bool CreateDirectory(const char* path);

        // Delete file
        bool DeleteFile(const char* path);

        // Delete directory
        bool DeleteDirectory(const char* path);

        // Get file size
        int64_t GetFileSize(const char* path);

        // Get file attributes
        FileAttributes GetFileAttributes(const char* path);

        // Copy file
        bool CopyFile(const char* source, const char* dest, bool overwrite = true);

        // Move file
        bool MoveFile(const char* source, const char* dest);

        // Get current working directory
        bool GetCurrentDirectory(char* buffer, size_t bufferSize);

        // Set current working directory
        bool SetCurrentDirectory(const char* path);

        // Path utilities
        void NormalizePath(char* path);  // Convert \ to / on Unix
        const char* GetFileName(const char* path);
        const char* GetFileExtension(const char* path);
        bool GetAbsolutePath(const char* relativePath, char* absolutePath, size_t bufferSize);

        // Directory iteration
        struct DirectoryEntry
        {
            char name[256];
            bool isDirectory;
            int64_t size;
        };

        typedef void* DirectoryHandle;

        DirectoryHandle OpenDirectory(const char* path);
        bool ReadDirectory(DirectoryHandle handle, DirectoryEntry& entry);
        void CloseDirectory(DirectoryHandle handle);
    }
}
