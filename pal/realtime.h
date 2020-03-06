#pragma once
#include <functional>
#include <portaudio.h>
#include <condition_variable>
#include <mutex>


/// Enables simple real-time audio processing.
class Realtime
{
    public:
    /// Creates a new Realtime audio instance
    Realtime();

    float getCpuLoad();

    /// Starts the realtime audio stream with the given audio processing
    /// callback.
    /// @param  callback    The audio callback
    void start(std::function<void(int frames, int channels, float *in, float *out)> callback);

    /// Stops the current audio stream. Causes a call to `wait` to continue.
    void stop();

    // Wait until audio is stopped.
    void wait();

    bool isStopped()
    {
        return stopped;
    }

    std::function<void(int frames, int channels, float *in, float *out)> callback;
    double startTime;

    bool stopped;
    int count;
    PaStream *stream;

    friend int paCallback(
        const void *inputBuffer,
        void *outputBuffer,
        unsigned long framesPerBuffer,
        const PaStreamCallbackTimeInfo* timeInfo,
        PaStreamCallbackFlags statusFlags,
        void *userData);
};

Realtime quickAudio(std::function<void(int frames, int channels, float *in, float *out)>  f);
