#include "GranularSynth.hpp"
#include "../../Core/Log.hpp"
#include "../../Core/Engine.hpp"
#include "imgui.h"
#include "InstrumentsUtils.hpp"
#include <cstdlib>
#include <ctime>

namespace MSQ
{
	GranularSynth::Grain::Grain(Envelope::AHRInfo env, Sample* sample, int position,float speed, float pan)
	: Playable(2), _envelope(env), _sampler(2)
	{
		_sampler.SetSample(sample);
		_sampler.SetSpeed(speed);
		_sampler.SetPosition(position);
		_pan = pan;
	}

	void GranularSynth::Grain::Play(int samples)
	{   
		_sampler.Play(samples);
		_envelope.Play(samples);
		const int* samplerBuffer = _sampler.GetBuffer();
		const float* envBuffer = _envelope.GetBuffer();

		for(int i = 0; i < samples; i++)
		{
			int i2 = i*2;
			Util::Stereo s = Util::Pan(samplerBuffer[i2] * envBuffer[i], samplerBuffer[i2 + 1] * envBuffer[i/2], _pan);
			_buffer[i2] = s.Left;
			_buffer[i2+1] = s.Right;
		}
	}

	bool GranularSynth::Grain::GetDone()
	{
		return _envelope.GetDone();
	}

	void GranularSynth::Grain::SetPosition(int p)
	{
		_sampler.SetPosition(p);
	}

	GranularSynth::Voice::Voice(GranularSynth* parent, unsigned char note, unsigned char vel)
	: Playable(2), _parent(parent), _envelope(parent->GetVoiceEnv()),_note(note), _vel(vel), _timer(0) {}

	void GranularSynth::Voice::Play(int samples)
	{
		int density = (float)Engine::Instance()->GetSampleRate()/_parent->GetDensity();

		if(samples > _bufferSize)
			return;
		if(_timer > density)
			_timer = 0;

		EmptyBuffer();
		int i = 0;
		while(i < samples)
		{
			int next = density - (_timer + i);
			int lengthToEval = std::min(next - i, samples - i);
			if (lengthToEval < 0)
				break;
			for(GranularSynth::Grain* g : _grains)
			{
				g->Play(lengthToEval);
				const int* gBuffer = g->GetBuffer();
				for (int j = 0; j < lengthToEval; j++)
					for(int k = 0; k < _outputChannels; k++)
						_buffer[(i + j) * _outputChannels + k] += gBuffer[j * _outputChannels + k] * _parent->_grainVolume;
			}
			_timer += lengthToEval;
			i += lengthToEval;
			if (i < samples)
			{
				_timer = 0;
				GranularSynth::Grain* g = new GranularSynth::Grain(
					_parent->GetGrainEnv(), 
					_parent->GetSample(), 
					(_parent->_position + i) + _parent->_randomPos * (-1.f + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(2.f)))) * Engine::Instance()->GetSampleRate(),
					_parent->_defaultGrainSpeed * _parent->_grainSpeed,
					_parent->_randomPan * (-1.f + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(2.f)))));
				_grains.emplace_back(g);
			}
		}
		_envelope.Play(samples);
		const float* envBuff = _envelope.GetBuffer();
		for(int j = 0; j < samples; j++)
			for(int k = 0; k < _outputChannels; k++)
				_buffer[j * _outputChannels + k] = (int)(envBuff[j] * (float)_buffer[j * _outputChannels + k]);
		for(int j = 0; j < _grains.size(); j++)
			if(_grains[j]->GetDone())
				_grains.erase(_grains.begin() + j);
	}

	const std::vector<GranularSynth::Grain*>& GranularSynth::Voice::GetGrains()
	{
		return _grains;
	}

	GranularSynth::Voice::~Voice()
	{

	}

	bool GranularSynth::Voice::GetDone()
	{
		return _envelope.GetDone();
	}

	int GranularSynth::Voice::GetGrainCount()
	{
		return _grains.size();
	}

	int GranularSynth::Voice::GetTimer()
	{
		return _timer;
	}

	void GranularSynth::Play(int samples)
	{
		if(samples > _bufferSize)
			return;
		EmptyBuffer();
		for(Voice& v : _voices)
		{
			v.Play(samples);
			const int* vBuffer = v.GetBuffer();
			int vOutputChannels = v.GetOutputChannels();
			for(int i = 0; i < samples; i++)
			 	for(int j = 0; j < vOutputChannels; j++)
					_buffer[(i * vOutputChannels) + j] += vBuffer[(i * vOutputChannels) + j];
		}

		for(auto it = _voices.begin(); it != _voices.end(); it++)
			if(it->GetDone())
				_voices.erase(it);
		_position += samples * _speed * _defaultGrainSpeed;
	}

	void GranularSynth::NoteOn(unsigned char note, unsigned char vel)
	{
		_voices.emplace_back(this, note, vel);
	}

	void GranularSynth::NoteOff(unsigned char note, unsigned char vel)
	{

	}

	Sample* GranularSynth::GetSample()
	{
		return _sample;
	}
	
	Envelope::ADSRInfo GranularSynth::GetVoiceEnv()
	{
		return _voiceEnv;
	}


	Envelope::AHRInfo GranularSynth::GetGrainEnv()
	{
		return _grainEnv;
	}

	float GranularSynth::GetDensity()
	{
		return _density;
	}

	GranularSynth::GranularSynth(Sample* sample)
	: Instrument(2)
	{
		_position = 0.f;
		_speed = 0.f;
		_density = 0.5f;
		_grainEnv.a = 10000;
		_grainEnv.h = 10000;
		_grainEnv.r = 10000;

		_voiceEnv.a = 100;
		_voiceEnv.d = 100;
		_voiceEnv.s = 0.2f;
		_voiceEnv.r = 100;
		_randomPan = 0.f;
		_grainVolume = 0.125f;

		_sample = sample;
		_defaultGrainSpeed = (float)sample->GetSampleRate() / (float)Engine::Instance()->GetSampleRate();
		_grainSpeed = 1;
	}

	GranularSynth::GranularSynth()
	: Instrument(2)
	{
		srand (static_cast <unsigned> (time(0)));
		_position = 0.f;
		_speed = 0.f;
		_density = 0.5;
		_grainEnv.a = 10000;
		_grainEnv.h = 10000;
		_grainEnv.r = 1000;

		_voiceEnv.a = 100;
		_voiceEnv.d = 100;
		_voiceEnv.s = 0.2f;
		_voiceEnv.r = 100;

		_sample = nullptr;
		_defaultGrainSpeed = 1.f;
		_grainSpeed = 1;
		_randomPan = 0.f;
		_grainVolume = 0.125f;
	}


	void GranularSynth::SetRandomPan(float p)
	{
		_randomPan = p;
	}

	float GranularSynth::GetRandomPan()
	{
		return _randomPan;
	}
	
	void GranularSynth::SetDensity(float d)
	{
		_density = d;
	}

	int GranularSynth::GetPosition()
	{
		return _position;
	}

	void GranularSynth::SetSample(Sample* s)
	{
		_defaultGrainSpeed = (float)s->GetSampleRate() / (float)Engine::Instance()->GetSampleRate();

		_sample = s;
	}

	void GranularSynth::SetSpeed(float s)
	{
		_speed = s;
	}

	void GranularSynth::SetPosition(int p)
	{
		_position = p;
	}

	float GranularSynth::GetSpeed()
	{
		return _speed;
	}

	void GranularSynth::Render()
	{
		bool v;
		ImGui::Begin("GranularSynth");
		ImGui::Text("%d", _voices.front().GetGrainCount());
		ImGui::Text("%d", _voices.front().GetTimer());
		ImGui::SliderInt("Position", &_position, 0, _sample->GetLength());
		ImGui::SliderFloat("Speed", &_speed, -10, 10);
		ImGui::SliderFloat("Grains per second", &_density, 0.1f, 100.f);
		ImGui::SliderInt("Grain A", &_grainEnv.a, 0, 100000);
		ImGui::SliderInt("Grain H", &_grainEnv.h, 0, 100000);
		ImGui::SliderInt("Grain R", &_grainEnv.r, 0, 100000);
		ImGui::SliderFloat("GrainSpeed", &_grainSpeed, -2, 2);
		ImGui::SliderFloat("RandomPan", &_randomPan, 0.f, 1.f);
		ImGui::SliderFloat("RandomPos", &_randomPos, 0.f, 1.f);
		ImGui::SliderFloat("GrainVolume", &_grainVolume, 0.f, 0.125f);
		ImGui::End();
	}
}
