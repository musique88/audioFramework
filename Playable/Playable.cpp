#include "Playable.hpp"
#include "../Core/Engine.hpp"

namespace MSQ
{
	Playable::Playable(int outputChannels)
	{
		_bufferSize = Engine::Instance()->GetBufferSize();
		_outputChannels = outputChannels;
		_active = true;
		_buffer = new float[_bufferSize * outputChannels];
		_name = "Placeholder Playable";
	}

	Playable::~Playable()
	{
		delete _buffer;
	}

	void Playable::SetName(const char* s)
	{
		_name = s;
	}

	const std::string Playable::GetName() const
	{
		return _name;
	}

	const float* const & Playable::GetBuffer() const
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

	bool& Playable::GetActive()
	{
		return _active;
	}
}