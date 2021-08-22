#include "InstrumentsUtils.hpp"
#include <algorithm>
#include <iostream>

namespace MSQ::Util
{
	Stereo Pan(float left, float right, float pan)
	{
		return {(std::min(1.f, 1.f-pan) * left),(std::min(1.f, 1.f+pan) * left)};
	}
}