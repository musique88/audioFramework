#include "Sampler.hpp"
#include <algorithm>

namespace MSQ
{
    Sampler::Sampler(int maxBufferSize, int outputChannels)
    :Playable(maxBufferSize, outputChannels)
    {
        _sample = nullptr;
        _position = 0;
    }

    const bool Sampler::IsEmpty() const
    {
        return _sample == nullptr;
    }

    void Sampler::SetSample(Sample* s)
    {
        if (s->GetChannels() != _outputChannels)
            throw 1;
        _sample = s;
    }

    const Sample& Sampler::GetSample() const
    {
        return *_sample;
    }

    void Sampler::SetPosition(const int& p)
    {
        _position = p;
    }

    const int& Sampler::GetPosition() const
    {
        return _position;
    }

    void Sampler::Reset()
    {
        _position = 0;    
    }

    const bool Sampler::IsDone() const
    {
        return _position > _sample->GetLength();
    }


    // TODO
    void Sampler::Play(int samples)
    {
        if(samples > _bufferSize)
            return;
        
        const std::vector<int>& sampleArray = _sample->GetArray();
        int positionInArray = _position * _outputChannels;

        int indexEndOfSampleArray = samples*_outputChannels;
        for(int i = 0; i < indexEndOfSampleArray; i++)
            _buffer[i] = sampleArray[i + positionInArray];
        _position += samples;
    }
}