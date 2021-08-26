#pragma once
#include "../../Core/Sample.hpp"
#include "Instrument.hpp"

namespace MSQ
{
	class Sampler : public Instrument
	{
	public:
		Sampler(int outputChannels);
		const bool IsEmpty() const;
		void SetSample(Sample* s);
		const Sample& GetSample() const;
		virtual void SetPosition(const int& p);
		const int& GetPosition() const;
		void Reset();
		virtual void Play(int samples) override;
		void SetSpeed(float _speed);
		void NoteOn(unsigned char note, unsigned char vel) override;
		void NoteOff(unsigned char note, unsigned char vel) override;
		void Render() override;

	protected:
		const Sample* _sample;
		bool _selectingSample;
		int _position;
		float _speed;
		int _notesOn;
	};
}