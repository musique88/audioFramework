#include "Sampler.hpp"
#include <algorithm>
#include "../Core/Log.hpp"

namespace MSQ
{
	Sampler::Sampler(int outputChannels)
	:Instrument(outputChannels)
	{
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
		if (s->GetLength() > _position)
			_active = true;
		_sample = s;
		
	}

	const Sample& Sampler::GetSample() const
	{
		return *_sample;
	}

	void Sampler::SetPosition(const int& p)
	{
		_active = p < _sample->GetLength();
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

	void Sampler::Play(int samples)
	{
		EmptyBuffer();
		if(samples > _bufferSize || _notesOn <= 0)
			return;
		
		const std::vector<float>& sampleArray = _sample->GetArray();
		int positionInArray = _position * _outputChannels;
		int remainingSamples = samples;
		remainingSamples = std::max(0, remainingSamples);
		MSQ::Log::Instance()->Info(std::to_string(remainingSamples));

		for(int i = 0; i < remainingSamples; i++)
			for(int j = 0; j < _outputChannels; j++)
				_buffer[i * _outputChannels + j] = sampleArray[((int)(_speed * i) * _outputChannels) + positionInArray + j];
		for(int i = remainingSamples; i < samples - remainingSamples; i++)
			for(int j = 0; j < _outputChannels; j++)
				_buffer[i * _outputChannels + j] = 0;
		_position += samples * _speed;
	}

	void Sampler::SetSpeed(float s)
	{
		_speed = s;
	}
	
	void Sampler::NoteOn(unsigned char note, unsigned char vel)
	{
		_notesOn ++;
	}

	void Sampler::NoteOff(unsigned char note, unsigned char vel)
	{
		_notesOn --;
	}
}