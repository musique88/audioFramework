#pragma once
#include "Sampler.hpp"

namespace MSQ
{
	class LoopingSampler : public Sampler
	{
	public:
		LoopingSampler(int);
		void Play(int) override;
		void SetPosition(const int&) override;
	};
}
