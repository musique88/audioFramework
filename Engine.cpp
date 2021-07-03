#include "Engine.hpp"
#include <sstream>
#include "Log.hpp"

namespace MSQ
{
    Engine* Engine::_instance = nullptr;

    Engine::Engine()
    {
        Pa_Initialize();
    }

    Engine::~Engine()
    {
        for (int i = 0; i < (int)_instruments.size(); i ++)
            delete _instruments[i];
    }

    Engine* Engine::Instance()
    {
        if (_instance == nullptr)
            _instance = new Engine();
        return _instance;
    }

    void Engine::LogDevices() const
    {
        int devices = Pa_GetDeviceCount();
        for (int i= 0; i < devices; i++)
        {
            std::stringstream s;
            const PaDeviceInfo* info = Pa_GetDeviceInfo(i);
            s << i << ": " << info->name << std::endl << 
                "Default sample rate: " << info->defaultSampleRate << 
                " IN: " << info->maxInputChannels << 
                " OUT: " << info->maxOutputChannels;
            MSQ::Log::instance()->Info(s.str());
        }
    }


    void Engine::OpenStream(int inIndex, int outIndex, int outChannels, int sampleRate, int bufferLength)
    {
        PaStreamParameters inR;
        PaStreamParameters* in = &inR;
        if (inIndex >= 0)
        {
            const PaDeviceInfo *inDeviceInfo = Pa_GetDeviceInfo(inIndex);
            in->channelCount = inDeviceInfo->maxOutputChannels;
            in->device = inIndex;
            in->hostApiSpecificStreamInfo = NULL;
            in->sampleFormat = paInt32;
            in->suggestedLatency = inDeviceInfo->defaultLowOutputLatency;
        }
        else
            in = nullptr;
        PaStreamParameters outR;
        PaStreamParameters* out = &outR;
        if (outIndex >= 0)
        {
            const PaDeviceInfo *outDeviceInfo = Pa_GetDeviceInfo(outIndex);
            out->channelCount = outChannels;
            _outputChannels = outChannels;
            out->device = outIndex;
            out->hostApiSpecificStreamInfo = NULL;
            out->sampleFormat = paInt32;
            out->suggestedLatency = outDeviceInfo->defaultLowOutputLatency;
        }
        else
            out = nullptr;
        std::stringstream s;
        s << "Opening stream with " << Pa_GetDeviceInfo(outIndex)->name << " as output device with " << outChannels << " channels" << std::endl;
        s << "The selected sample rate is " << sampleRate << " and the buffer size is " << bufferLength;

        Log::instance()->Info(s.str());

        Pa_OpenStream(  &_stream,
                        in,
                        out,
                        sampleRate,
                        bufferLength,
                        paNoFlag,
                        StreamCallback,
                        this);
    }

    int Engine::StreamCallback(const void* inputData, void* outputBuffer,
                    unsigned long framesPerBuffer,
                    const PaStreamCallbackTimeInfo* timeInfo,
                    PaStreamCallbackFlags statusFlag,
                    void* userData)
    {
        Engine* engine = (Engine*) userData;
        int* out = (int*)outputBuffer;
        int framesToFill = framesPerBuffer * engine->_outputChannels;
        for (int i = 0; i < framesToFill; i++)
            out[i] = 0;
        for(Playable* p : engine->_instruments)
        {
            p->Play(framesPerBuffer);
            for (int i = 0; i < framesToFill; i ++)
                out[i] += p->GetBuffer()[i]/2;
        }

        for(int i = 0; i < framesToFill; i++)
            out[i] /= 16;
        return paContinue;
    }

    void Engine::StartStream()
    {
        Pa_StartStream(_stream);
    }
    
    void Engine::StopStream()
    {
        Pa_StopStream(_stream);
    }

    void Engine::AddInstrument(Playable* p)
    {
        _instruments.push_back(p);
    }

    void Engine::Hang(int seconds)
    {
        Pa_Sleep(seconds*1000);
    }
}