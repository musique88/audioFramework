#pragma once
#include "../Playable.hpp"
#include <vector>

namespace MSQ
{

	class Instrument : public Playable
	{

	public:
		virtual void NoteOn(unsigned char note, unsigned char vel) = 0;
		virtual void NoteOff(unsigned char note, unsigned char vel) = 0;
		Instrument(int channels);
	};

}