#include "Sampler.hpp"
#include <algorithm>

namespace MSQ
{
    Sampler::Sampler(int maxBufferSize, int outputChannels)
    :Playable(maxBufferSize, outputChannels)
    {
        _isDone = false;
        _speed = 1.f;
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
        _isDone = p > _sample->GetLength();
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
        return _isDone;
    }


    // TODO
    void Sampler::Play(int samples)
    {
        if(samples > _bufferSize || _isDone)
            return;
        
        const std::vector<int>& sampleArray = _sample->GetArray();
        int positionInArray = _position * _outputChannels;
        int remainingSamples = samples;
        if (_sample->GetLength() < samples + _position)
        {
            EmptyBuffer();
            remainingSamples = _sample->GetLength() - _position;
            _isDone = true;
        }

        for(int i = 0; i < remainingSamples; i++)
            for(int j = 0; j < _outputChannels; j++)
                _buffer[i * _outputChannels + j] = sampleArray[((int)(_speed * i) * _outputChannels) + positionInArray + j];
        _position += samples * _speed;
    }

    void Sampler::SetSpeed(float s)
    {
        _speed = s;
    }
}