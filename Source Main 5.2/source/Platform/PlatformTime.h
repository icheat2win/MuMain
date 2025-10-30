// PlatformTime.h - Cross-platform time/timer functions
// Replaces GetTickCount, QueryPerformanceCounter, etc.

#pragma once

#include <cstdint>

namespace Platform
{
    // High-resolution timer class
    class Timer
    {
    public:
        Timer();

        // Start/restart timer
        void Start();

        // Stop timer
        void Stop();

        // Reset timer
        void Reset();

        // Get elapsed time in seconds
        double GetElapsedSeconds() const;

        // Get elapsed time in milliseconds
        double GetElapsedMilliseconds() const;

        // Get elapsed time in microseconds
        int64_t GetElapsedMicroseconds() const;

        // Is timer running?
        bool IsRunning() const { return m_Running; }

    private:
        int64_t m_StartTime;
        int64_t m_StopTime;
        bool m_Running;

        static int64_t GetCurrentTimeMicroseconds();
    };

    // Time utilities
    namespace Time
    {
        // Get current time in milliseconds since program start
        uint64_t GetTickCount();

        // Get current time in microseconds (high precision)
        int64_t GetTimeMicroseconds();

        // Get current time in seconds since program start
        double GetTimeSeconds();

        // Sleep/delay functions
        void Sleep(uint32_t milliseconds);
        void SleepMicroseconds(uint64_t microseconds);

        // Frame timing
        class FrameTimer
        {
        public:
            FrameTimer();

            // Update (call once per frame)
            void Update();

            // Get delta time in seconds
            float GetDeltaTime() const { return m_DeltaTime; }

            // Get current FPS
            float GetFPS() const { return m_FPS; }

            // Get average FPS (over last second)
            float GetAverageFPS() const { return m_AverageFPS; }

            // Set target FPS (0 = unlimited)
            void SetTargetFPS(float fps);

            // Get frame count
            uint64_t GetFrameCount() const { return m_FrameCount; }

        private:
            double m_LastTime;
            float m_DeltaTime;
            float m_FPS;
            float m_AverageFPS;
            float m_TargetFPS;
            uint64_t m_FrameCount;

            // For FPS calculation
            float m_FPSAccumulator;
            uint32_t m_FPSFrameCount;
            double m_FPSLastTime;
        };
    }
}
