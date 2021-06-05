#pragma once
#include "Sample.hpp"
#include "Playable.hpp"

namespace MSQ
{
    class Sampler : public Playable
    {
    public:
        Sampler(int maxBufferSize, int outputChannels);
        const bool IsEmpty() const;
        void SetSample(Sample* s);
        const Sample& GetSample() const;
        void SetPosition(const int& p);
        const int& GetPosition() const;
        void Reset();
        const bool IsDone() const;
        void Play(int samples) override;

    protected:
        const Sample* _sample;
        int _position;
    };
}