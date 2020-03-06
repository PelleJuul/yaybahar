#include "realtime.h"
#include <functional>
#include <exception>
#include <portaudio.h>
#include <cmath>
#include <iostream>
#include <thread>
#include <chrono>
#include <cstring>
#include <unistd.h>
#if __APPLE__
#include <pa_mac_core.h>
#endif

int paCallback(
    const void *inputBuffer,
    void *outputBuffer,
    unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void *userData)
{
    Realtime *caller = (Realtime*)userData;
    auto *in = (float*)inputBuffer;
    auto *out = (float*)outputBuffer;

    if (caller->startTime < 0)
        caller->startTime = timeInfo->currentTime;

    if (caller->stopped)
    {
        memset(outputBuffer, 0, framesPerBuffer * 2);
        return 1;
    }

    if (Pa_GetStreamCpuLoad(caller->stream) > 0.9)
       std::cout << "Real-time audio consuming too much CPU time\n"; 

    float time = timeInfo->currentTime - caller->startTime;

    caller->callback(framesPerBuffer, 2, in, out);

    return 0;
}

Realtime::Realtime()
{
    auto err = Pa_Initialize();

    if (err != paNoError )
        throw std::runtime_error(Pa_GetErrorText(err));
}

void Realtime::start(std::function<void(int frames, int channels, float *in, float *out)> callback)
{
    this->callback = callback;

    PaStreamParameters inputParameters;
    PaStreamParameters outputParameters;
    inputParameters.device = Pa_GetDefaultInputDevice();
    inputParameters.channelCount = 2;
    inputParameters.sampleFormat = paFloat32;
    inputParameters.suggestedLatency = 0.005;
    inputParameters.hostApiSpecificStreamInfo = NULL;

    outputParameters.device = Pa_GetDefaultOutputDevice();
    outputParameters.channelCount = 2;
    outputParameters.sampleFormat = paFloat32;   
    outputParameters.suggestedLatency = 0.005; 
    outputParameters.hostApiSpecificStreamInfo = NULL;

#if __APPLE__
    PaMacCoreStreamInfo macCoreStreamInfo;
    PaMacCore_SetupStreamInfo(&macCoreStreamInfo, paMacCoreChangeDeviceParameters);
    inputParameters.hostApiSpecificStreamInfo = &macCoreStreamInfo;
    outputParameters.hostApiSpecificStreamInfo = &macCoreStreamInfo;
#endif

    auto err = Pa_OpenStream(
        &this->stream,
        &inputParameters,
        &outputParameters,
        44100,
        512,
        paClipOff,
        paCallback,
        this);

    if (err != paNoError)
        throw std::runtime_error(Pa_GetErrorText(err));

    err = Pa_StartStream(stream);

    if (err != paNoError)
        throw std::runtime_error(Pa_GetErrorText(err));

    stopped = false;
    count = 0;
    startTime = -1;
}

float Realtime::getCpuLoad()
{
    return Pa_GetStreamCpuLoad(stream);
}

void Realtime::wait()
{
    while(!stopped)
    {
        usleep(100e3);
    }
}

void Realtime::stop()
{
    stopped = true;
}

Realtime quickAudio(std::function<void(int frames, int channels, float *in, float *out)> f)
{
    Realtime rt;
    rt.start(f);
    return rt;
};
