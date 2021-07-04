#pragma once

namespace MSQ
{
    class Playable
    {
    protected:
        int* _buffer;
        int _bufferSize;
        int _lastSamplesBufferPlay;
        int _outputChannels;
        bool _active;
    public:
        Playable(int bufferSize, int outputChannels);
        virtual ~Playable();
        virtual void Play(int samples) = 0;
        void EmptyBuffer();
        const int* const & GetBuffer() const;
        const int& GetBufferSize() const;
        const int& GetOutputChannels() const;
        const bool& GetActive() const;
    };
}