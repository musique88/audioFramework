#include "Sampler.hpp"
#include <algorithm>
#include "../Core/Log.hpp"
#include "../Core/Engine.hpp"

namespace MSQ
{
	Sampler::Sampler(int outputChannels)
	:Instrument(outputChannels)
	{
		_speed = 1.f;
		_sample = nullptr;
		_position = 0;
		_selectingSample = true;
		_name = "Sampler";
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

		for(int i = 0; i < remainingSamples; i++)
			for(int j = 0; j < _outputChannels; j++)
				_buffer[i * _outputChannels + j] = sampleArray[((int)(_speed * i) * _outputChannels) + positionInArray + j];
		for(int i = remainingSamples; i < samples - remainingSamples; i++)
			for(int j = 0; j < _outputChannels; j++)
				_buffer[i * _outputChannels + j] = 0;
		SetPosition(_position + samples * _speed);
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

	void Sampler::Render()
	{
		ImGui::PushID(_id);
		ImGui::Begin((std::string("Sampler##") + std::to_string(_id)).c_str());
		if (_sample == nullptr)
		{
			_selectingSample = true;
			goto end;
		}
		ImGui::Text("%s", _sample->GetFilePath());
		if (ImGui::SliderInt("Position", &_position, 0, _sample->GetLength()))
			SetPosition(_position);
		ImGui::SliderFloat("Speed", &_speed, -10, 10);
		ImGui::Text("%d", _notesOn);
	end:
		ImGui::End();

		if (_selectingSample)
		{
			ImGui::Begin((std::string("Select Sample##") + std::to_string(_id)).c_str(), &_selectingSample);
			const std::vector<Sample*> samples = Engine::Instance()->GetSamples();
			for (int i = 0; i < samples.size(); i++)
			{
				ImGui::Text("%02d :: %s", i, samples[i]->GetFilePath());
				ImGui::SameLine();
				if (ImGui::Button((std::string("Select##") + std::to_string(i)).c_str()))
				{
					SetSample(samples[i]);
					_selectingSample = false;
				}
			}
			ImGui::End();
		}

		ImGui::PopID();
	}
}