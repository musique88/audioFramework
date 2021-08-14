#include "InstrumentsUtils.hpp"
#include <algorithm>
#include <iostream>

namespace MSQ::Util
{
	Stereo Pan(int left, int right, float pan)
	{
		return {(int)(std::min(1.f, 1.f-pan) * left), (int)(std::min(1.f, 1.f+pan) * left)};
	}
}