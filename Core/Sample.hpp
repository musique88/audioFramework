#pragma once
#include <vector>
#include <string>

namespace MSQ
{
	class Sample
	{
		int _channels;
		std::vector<float> _buffer;
		int _sampleRate;
		std::string _path;
	public: 
		Sample(const int& channels, const float* buffer, const int& bufferSize, int sampleRate);
		Sample(int channels, const std::vector<float> buffer, int sampleRate);
		Sample(const char* file);
		const int GetChannels() const;
		const int GetLength() const;
		const int GetSampleRate() const;
		const std::vector<float>& GetArray() const;
		const char* GetFilePath() const;
	};
}