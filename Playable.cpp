#include "Playable.hpp"

namespace MSQ
{
    Playable::Playable(int bufferSize, int outputChannels)
    : _bufferSize(bufferSize), _outputChannels(outputChannels)
    {
        _buffer = new int[bufferSize * outputChannels];
    }

    Playable::~Playable()
    {
        delete _buffer;
    }

    const int* const & Playable::GetBuffer() const
    {
        return _buffer;
    }

    const int& Playable::GetBufferSize() const
    {
        return _bufferSize;
    }

    const int& Playable::GetOutputChannels() const
    {
        return _outputChannels;
    }

}