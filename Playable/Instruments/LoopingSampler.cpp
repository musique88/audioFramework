#include "LoopingSampler.hpp"


namespace MSQ
{
	LoopingSampler::LoopingSampler(int channels)
	:Sampler(channels) {
		_name = "Looping Sampler";
	}

	void LoopingSampler::Play(int samples)
	{
		EmptyBuffer();
		if(samples > _bufferSize || _notesOn <= 0 || _speed == 0)
			return;
		
		const std::vector<float>& sampleArray = _sample->GetArray();
		int remainingSamples = std::min(samples, _sample->GetLength() - (int)(_position + samples * _speed));
		remainingSamples = std::min(remainingSamples,(int)(_position + samples * _speed));
		remainingSamples = std::max(0, remainingSamples);

		for(int i = 0; i < remainingSamples; i++)
			for(int j = 0; j < _outputChannels; j++)
				_buffer[i * _outputChannels + j] = sampleArray[((int)(_speed * i + _position) * _outputChannels) + j];
		int endpoint = samples - remainingSamples;
		if (_speed >= 0)
			for(int i = remainingSamples; i < endpoint; i++)
				for(int j = 0; j < _outputChannels; j++)
					_buffer[i * _outputChannels + j] = sampleArray[((int)(_speed * i) * _outputChannels) + j];
		else
			for(int i = remainingSamples; i < endpoint; i++)
				for(int j = 0; j < _outputChannels; j++)
					_buffer[i * _outputChannels + j] = sampleArray[((int)(_speed * i + _sample->GetLength()) * _outputChannels) + j];
		SetPosition(_position + samples * _speed);
	}

	void LoopingSampler::SetPosition(const int& p )
	{
		if(p < 0)
			_position += _sample->GetLength();
		else
			_position = p % _sample->GetLength();
	}
}
