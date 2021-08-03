#include "GranularSynth.hpp"
#include "../../Core/Log.hpp"

namespace MSQ
{
	GranularSynth::Grain::Grain(Envelope::AHRInfo env, Sample* sample, float speed)
	: Playable(2), _envelope(env), _sampler(2)
	{
		_sampler.SetSample(sample);
		_sampler.SetSpeed(speed);
		_sampler.SetPosition(100000);
	}

	void GranularSynth::Grain::Play(int samples)
	{   
		_sampler.Play(samples);
		_envelope.Play(samples);
		const int* samplerBuffer = _sampler.GetBuffer();
		const float* envBuffer = _envelope.GetBuffer();

		for(int i = 0; i < samples; i++)
			for(int j = 0; j < _outputChannels; j++)
				_buffer[i * _outputChannels + j] = samplerBuffer[i * _outputChannels + j] * envBuffer[i];
	}

	bool GranularSynth::Grain::GetDone()
	{
		return _envelope.GetDone();
	}

	GranularSynth::Voice::Voice(GranularSynth* parent, unsigned char note, unsigned char vel)
	: Playable(2), _parent(parent), _envelope(parent->GetVoiceEnv()),_note(note), _vel(vel), _density(parent->GetDensity()), _timer(0) {}

	void GranularSynth::Voice::Play(int samples)
	{
		Log::Instance()->Info(std::to_string(_grains.size()));
		if(samples > _bufferSize)
			return;

		EmptyBuffer();
		int i = 0;
		while(i < samples)
		{
			int next =  _density - (_timer + i);
			int lengthToEval = std::min(next - i, samples - i);
			if (lengthToEval < 0)
				break;
			for(GranularSynth::Grain* g : _grains)
			{
				g->Play(lengthToEval);
				const int* gBuffer = g->GetBuffer();
				for (int j = 0; j < lengthToEval; j++)
					for(int k = 0; k < _outputChannels; k++)
						_buffer[(i + j) * _outputChannels + k] += gBuffer[j * _outputChannels + k] / 16;
			}
			_timer += lengthToEval;
			i += lengthToEval;
			if (i < samples)
			{
				_timer = 0;
				_grains.emplace_back(new Grain(_parent->GetGrainEnv(), _parent->GetSample(), 1));
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


	bool GranularSynth::Voice::GetDone()
	{
		return _envelope.GetDone();
	}

	GranularSynth::Voice::~Voice()
	{
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

	int GranularSynth::GetDensity()
	{
		return _density;
	}

	GranularSynth::GranularSynth(Sample* sample)
	: Instrument(2)
	{
		_density = 10000;
		_grainEnv.a = 10000;
		_grainEnv.h = 100000;
		_grainEnv.r = 100;

		_voiceEnv.a = 100;
		_voiceEnv.d = 100;
		_voiceEnv.s = 1.f;
		_voiceEnv.r = 100;

		_sample = sample;
		
	}

	void GranularSynth::SetDensity(int d)
	{
		_density = d;
	}

}
