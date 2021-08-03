#include "Playable.hpp"
#include "../Core/Engine.hpp"

namespace MSQ
{
	Playable::Playable(int outputChannels)
	{
		_bufferSize = Engine::Instance()->GetBufferSize();
		_outputChannels = outputChannels;
		_active = true;
		_buffer = new int[_bufferSize * outputChannels];
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

	void Playable::EmptyBuffer()
	{
		int totalIndexes = _outputChannels * _bufferSize;
		for (int i = 0 ; i < totalIndexes; i++)
			_buffer[i] = 0;
	}

	const bool& Playable::GetActive() const
	{
		return _active;
	}
}