#pragma once
#include <vector>
#include <string>
#include "Playable.hpp"
#include <portaudio.h>

namespace MSQ
{
    class Engine
    {
        PaStream* _stream;
        static Engine* _instance;
        static int StreamCallback(const void* inputData, void* outputBuffer,
                    unsigned long framesPerBuffer,
                    const PaStreamCallbackTimeInfo* timeInfo,
                    PaStreamCallbackFlags statusFlag,
                    void *userData);
        std::vector<Playable*> _instruments;
        Engine();
        ~Engine();
        int _outputChannels;
    public:
        static Engine* Instance();
        void LogDevices() const;
        void OpenStream(int inIndex, int outIndex, int outChannels = 2, int sampleRate = 44100, int bufferLength = 512);
        void StartStream();
        void StopStream();
        void Hang(int seconds);
        void AddInstrument(Playable* p);
    };
}