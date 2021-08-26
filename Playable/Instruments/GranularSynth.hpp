#pragma once

#include "Instrument.hpp"
#include "Envelope.hpp"
#include "LoopingSampler.hpp"
#include <list>

namespace MSQ
{
	class GranularSynth : public Instrument
	{
		class Grain : public Playable
		{
			Envelope::AHR _envelope;
			LoopingSampler _sampler;
			float _pan;
		public:
			Grain(Envelope::AHRInfo env, Sample* sample, int position,float speed, float pan = 0);
			void SetPosition(int p);
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
		public:
			Voice(GranularSynth* parent, unsigned char note, unsigned char vel);
			~Voice();
			unsigned char GetNote() const;
			void Play(int samples) override;
			void Done();
			bool GetDone();
			int GetGrainCount();
			const std::vector<Grain*>& GetGrains();
			int GetTimer();
		};

		std::vector<Voice*> _voices;
		Sample* _sample;
		Envelope::ADSRInfo _voiceEnv;
		Envelope::AHRInfo _grainEnv;
		float _density; //density as grains generated per second
		int _position;
		float _speed;
		float _defaultGrainSpeed;
		float _grainSpeed;
		float _randomPan;
		float _randomPos;
		float _grainVolume;

		bool _selectingSample;

	public:
		GranularSynth();
		GranularSynth(Sample* sample);
		void SetSample(Sample* sample);
		void SetDensity(float);
		void SetSpeed(float s);
		void SetPosition(int p);
		Sample* GetSample();
		Envelope::ADSRInfo GetVoiceEnv();
		Envelope::AHRInfo GetGrainEnv();
		float GetDensity();
		void Play(int samples) override;
		void NoteOn(unsigned char note, unsigned char vel) override;
		void NoteOff(unsigned char note, unsigned char vel) override;
		int GetPosition();
		float GetSpeed();
		void Render() override;
		void SetRandomPan(float);
		void SetRandomPos(float);
		float GetRandomPan();
	};
}