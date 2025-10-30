// PlatformAudio.h - Cross-platform audio abstraction
// Supports OpenAL (preferred) and platform-specific APIs

#pragma once

#include <cstdint>

namespace Platform
{
    // Audio format
    enum class AudioFormat
    {
        Mono8,
        Mono16,
        Stereo8,
        Stereo16
    };

    // Audio buffer handle
    typedef uint32_t AudioBufferHandle;
    constexpr AudioBufferHandle INVALID_AUDIO_BUFFER = 0;

    // Audio source handle
    typedef uint32_t AudioSourceHandle;
    constexpr AudioSourceHandle INVALID_AUDIO_SOURCE = 0;

    // Audio manager
    class Audio
    {
    public:
        // Initialize audio system
        static bool Initialize();

        // Shutdown audio system
        static void Shutdown();

        // Master volume (0.0 - 1.0)
        static void SetMasterVolume(float volume);
        static float GetMasterVolume();

        // Music volume (0.0 - 1.0)
        static void SetMusicVolume(float volume);
        static float GetMusicVolume();

        // Sound effects volume (0.0 - 1.0)
        static void SetSFXVolume(float volume);
        static float GetSFXVolume();

        // Create audio buffer from memory
        static AudioBufferHandle CreateBuffer(const void* data, size_t size, AudioFormat format, int sampleRate);

        // Create audio buffer from file
        static AudioBufferHandle CreateBufferFromFile(const char* filename);

        // Destroy audio buffer
        static void DestroyBuffer(AudioBufferHandle buffer);

        // Create audio source
        static AudioSourceHandle CreateSource();

        // Destroy audio source
        static void DestroySource(AudioSourceHandle source);

        // Attach buffer to source
        static void SetSourceBuffer(AudioSourceHandle source, AudioBufferHandle buffer);

        // Source properties
        static void SetSourceVolume(AudioSourceHandle source, float volume);
        static void SetSourcePitch(AudioSourceHandle source, float pitch);
        static void SetSourceLooping(AudioSourceHandle source, bool looping);
        static void SetSourcePosition(AudioSourceHandle source, float x, float y, float z);

        // Source playback control
        static void PlaySource(AudioSourceHandle source);
        static void PauseSource(AudioSourceHandle source);
        static void StopSource(AudioSourceHandle source);

        // Source state
        static bool IsSourcePlaying(AudioSourceHandle source);
        static bool IsSourcePaused(AudioSourceHandle source);

        // Listener (camera) properties
        static void SetListenerPosition(float x, float y, float z);
        static void SetListenerOrientation(float atX, float atY, float atZ, float upX, float upY, float upZ);

        // One-shot sound playback (creates temporary source)
        static void PlaySound(AudioBufferHandle buffer, float volume = 1.0f, bool looping = false);
        static void PlaySound3D(AudioBufferHandle buffer, float x, float y, float z, float volume = 1.0f);

        // Music playback (stream from file for large files)
        static bool PlayMusic(const char* filename, bool looping = true);
        static void StopMusic();
        static void PauseMusic();
        static void ResumeMusic();
        static bool IsMusicPlaying();

        // Update audio system (call once per frame)
        static void Update();

    private:
        Audio() = delete;
    };

    // Audio file loaders
    namespace AudioLoader
    {
        // Load WAV file
        bool LoadWAV(const char* filename, void** data, size_t* size, AudioFormat* format, int* sampleRate);

        // Load OGG Vorbis file (if available)
        bool LoadOGG(const char* filename, void** data, size_t* size, AudioFormat* format, int* sampleRate);

        // Free loaded audio data
        void FreeAudioData(void* data);
    }
}
