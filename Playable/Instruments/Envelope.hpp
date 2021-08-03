#pragma once

#include <vector>

namespace MSQ::Envelope
{
	class Base
	{
	protected:
		int _timer;
		float* _buffer;
		int _bufferSize;
	public:
		void EmptyBuffer();
		Base();
		virtual bool GetDone() = 0;
		virtual void Play(int samples) = 0;
		const float* GetBuffer() const;
	};

	struct ADSRInfo
	{
		int a, d;
		float s;
		int r;
	};

	class ADSR : public Base
	{
		int _a, _d;
		float _s;
		int _r;
		float _aSlope, _dSlope, _sSlope, _rSlope;
		bool _released;
	public:
		ADSR(int a, int d, float s, int r);
		ADSR(ADSRInfo i);
		void Release();
		void Play(int samples) override;
		bool GetDone() override;
	};

	struct AHRInfo
	{
		int a, h, r;
	};

	class AHR : public Base
	{
		int _a, _h, _r;
		float _aSlope, _hSlope, _rSlope;

	public:
		AHR(int a, int h, int r);
		AHR(AHRInfo i);
		void Play(int samples) override;
		bool GetDone() override;
	};

	struct ARInfo
	{
		int a, r;
	};

	class AR : public AHR
	{
	public:
		AR(int a, int r);
		AR(ARInfo i);
	};
}