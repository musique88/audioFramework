#pragma once
#include "../Core/Renderable.hpp"
#include <string>

namespace MSQ
{
	class Playable : public Renderable
	{
	protected:
		float* _buffer;
		int _bufferSize;
		int _outputChannels;
		bool _active;
		std::string _name;

	public:
		Playable(int outputChannels);
		virtual ~Playable();
		virtual void Play(int samples) = 0;
		void EmptyBuffer();

		void SetName(const char* name);
		const std::string GetName() const;
		const float* const & GetBuffer() const;
		const int& GetBufferSize() const;
		const int& GetOutputChannels() const;
		bool& GetActive();
	};
}