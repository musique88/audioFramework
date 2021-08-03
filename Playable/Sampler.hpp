#pragma once
#include "../Core/Sample.hpp"
#include "../Playable/Playable.hpp"

namespace MSQ
{
	class Sampler : public Playable
	{
	public:
		Sampler(int outputChannels);
		const bool IsEmpty() const;
		void SetSample(Sample* s);
		const Sample& GetSample() const;
		void SetPosition(const int& p);
		const int& GetPosition() const;
		void Reset();
		void Play(int samples) override;
		void SetSpeed(float _speed);

	protected:
		const Sample* _sample;
		int _position;
		float _speed;
	};
}