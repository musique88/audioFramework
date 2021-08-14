#pragma once
#include <vector>

namespace MSQ
{
	class Sample
	{
		int _channels;
		std::vector<int> _buffer;
		int _sampleRate;
	public: 
		Sample(const int& channels, const int* buffer, const int& bufferSize, int sampleRate);
		Sample(int channels, const std::vector<int> buffer, int sampleRate);
		Sample(const char* file);
		const int GetChannels() const;
		const int GetLength() const;
		const int GetSampleRate() const;
		const std::vector<int>& GetArray() const;
	};
}