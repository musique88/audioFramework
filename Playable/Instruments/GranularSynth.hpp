#pragma once

#include "Instrument.hpp"
#include "Envelope.hpp"
#include "../Sampler.hpp"
#include <list>

namespace MSQ
{
	class GranularSynth : public Instrument
	{
		class Grain : public Playable
		{
			Envelope::AHR _envelope;
			Sampler _sampler;
		public:
			Grain(Envelope::AHRInfo env, Sample* sample, float speed);
			void Play(int samples) override;
			bool GetDone();
		};

		class Voice : public Playable
		{
			Envelope::ADSR _envelope;
			unsigned char _note, _vel;
			std::vector<Grain*> _grains;
			GranularSynth* _parent;
			int _timer;
			int _density;
		public:
			Voice(GranularSynth* parent, unsigned char note, unsigned char vel);
			~Voice();
			void Play(int samples) override;
			bool GetDone();
		};

		std::list<Voice> _voices;
		Sample* _sample;
		Envelope::ADSRInfo _voiceEnv;
		Envelope::AHRInfo _grainEnv;
		int _density;

	public:
		GranularSynth(Sample* sample);
		void SetSample(Sample* sample);
		void SetDensity(int);
		Sample* GetSample();
		Envelope::ADSRInfo GetVoiceEnv();
		Envelope::AHRInfo GetGrainEnv();
		int GetDensity();
		void Play(int samples) override;
		void NoteOn(unsigned char note, unsigned char vel) override;
		void NoteOff(unsigned char note, unsigned char vel) override;
	};
}