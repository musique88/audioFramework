#include "Envelope.hpp"
#include <algorithm>
#include "../../Core/Engine.hpp"
#include "../../Core/Log.hpp"

namespace MSQ::Envelope
{
	Base::Base()
	{
		_timer = 0;
		_bufferSize = Engine::Instance()->GetBufferSize();
		_buffer = new float[_bufferSize];
	}

	void Base::EmptyBuffer()
	{
		for (int i = 0; i < _bufferSize; i++)
			_buffer[i] = 0.f;
	}

	const float* Base::GetBuffer() const
	{
		return _buffer;
	}

	ADSR::ADSR(int a, int d, float s, int r)
	{
		_released = false;
		_a = a;
		_d = d;
		_s = s;
		_r = r;
		_aSlope = 1.f/(float)_a;
		_dSlope = -1.f/(float)_d;
		_sSlope = 0.f;
		_rSlope = -_s/(float)_r;
	}

	ADSR::ADSR(ADSRInfo i)
	:ADSR(i.a, i.d, i.s, i.r){}

	void ADSR::Release()
	{
		_timer = 0;
		_released = true;
	}

	void ADSR::Play(int samples)
	{
		EmptyBuffer();
		int state;
		if (_released)
			state = _timer < _r ? 3 : 4;
		else
		{
			if (_timer > _a + _d)
				state = 2;
			else if (_timer > _a) // state d
				state = 1;
			else // state a
				state = 0;
		}
		int currentPosition = 0;
		while (currentPosition < samples)
		{
			float baseValue;
			float currentSlope;
			int positionInState;
			int deltaEnd;
			switch (state)
			{
				case 0:
					currentSlope = _aSlope;
					positionInState = _timer + currentPosition;
					deltaEnd = _a - positionInState;
					baseValue = 0;
					break;
				case 1:
					currentSlope = _dSlope;
					positionInState = _timer + currentPosition - _a;
					deltaEnd = _d - positionInState;
					baseValue = 1;
					break;
				case 2:
					currentSlope = _sSlope;
					positionInState = 0;
					deltaEnd = samples - currentPosition;
					baseValue = _s;
					break;
				case 3:
					currentSlope = _rSlope;
					positionInState = _timer + currentPosition;
					deltaEnd = _r - positionInState;
					baseValue = _s;
					break;
				default:
					goto end;
			}
			deltaEnd = std::min(deltaEnd, samples - currentPosition);
			for(int i = 0; i < deltaEnd; i++)
				_buffer[currentPosition + i] =
						currentSlope * (float) (positionInState + i) + baseValue;
			currentPosition += deltaEnd;
			if (state != 2)
				state++;
		}
		end:
		_timer += samples;
	}

	bool ADSR::GetDone()
	{
		return _released && _timer > _r;
	}

	AHR::AHR(int a, int h, int r)
	{
		_a = a;
		_h = h;
		_r = r;

		_aSlope = 1.f/(float)a;
		_hSlope = 0.f;
		_rSlope = -1.f/(float)r;
	}

	AHR::AHR(AHRInfo i)
	:AHR(i.a, i.h, i.r) {}

	void AHR::Play(int samples)
	{
		EmptyBuffer();
		int state;
		if (_timer > _a + _h + _r)
			return;
		else if (_timer >= _a + _h) // state r
			state = 2;
		else if (_timer >= _a) // state h
			state = 1;
		else // state a
			state = 0;
		int currentPosition = 0;
		while (currentPosition < samples)
		{
			float baseValue;
			float currentSlope;
			int positionInState;
			int deltaEnd;
			switch (state)
			{
				case 0:
					currentSlope = _aSlope;
					positionInState = _timer + currentPosition;
					deltaEnd = _a - positionInState;
					baseValue = 0;
					break;
				case 1:
					currentSlope = _hSlope;
					positionInState = _timer + currentPosition - _a;
					deltaEnd = _h - positionInState;
					baseValue = 1;
					break;
				case 2:
					currentSlope = _rSlope;
					positionInState = _timer + currentPosition - (_a + _h);
					deltaEnd = _r - positionInState;
					baseValue = 1;
					break;
				default:
					goto end;
					return;
			}
			deltaEnd = std::min(deltaEnd, samples - currentPosition);
			for(int i = 0; i < deltaEnd; i++)
			{
				_buffer[currentPosition + i] =
						currentSlope * (float) (positionInState + currentPosition + i) + baseValue;
				//Log::Instance()->Info(std::to_string(_buffer[currentPosition + i]));
			}
			currentPosition += deltaEnd;
			state++;
		}
		end:
		_timer += samples;
	}

	bool AHR::GetDone()
	{
		return _timer > _a + _h + _r;
	}

	AR::AR(int a, int r)
	:AHR(a,0,r){}

	AR::AR(ARInfo i)
	:AR(i.a, i.r) {}
}