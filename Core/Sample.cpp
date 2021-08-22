#include <sndfile.h>
#include "Sample.hpp"
#include "Log.hpp"
#include <string>

namespace MSQ
{
	Sample::Sample(const int& channels, const float* buffer, const int& bufferSize, int sampleRate)
	:_channels(channels)
	{
		_buffer = std::vector<float>(buffer, buffer + bufferSize);
		_sampleRate = sampleRate;
	}

	Sample::Sample(int channels, const std::vector<float> buffer, int sampleRate)
	:Sample(channels, &buffer[0], buffer.size(), sampleRate) {}

	const int Sample::GetChannels() const
	{
		return _channels;
	}

	const int Sample::GetLength() const
	{
		return _buffer.size()/GetChannels();
	}

	const std::vector<float>& Sample::GetArray() const
	{
		return _buffer;
	}

	Sample::Sample(const char* fileName)
	{
		struct SF_INFO info;
		SNDFILE* file = sf_open(fileName, SFM_READ, &info);
		if(int i = sf_error(file))
			Log::Instance()->Err(sf_error_number(i));
		Log::Instance()->Warn((std::string("File is of ") + std::to_string(info.samplerate)) + std::string(" hz"));
		float* arr = new float[info.frames * info.channels]; 
		sf_count_t size = sf_read_float(file, arr, info.frames * info.channels);
		if(int i = sf_error(file))
			Log::Instance()->Err(sf_error_number(i));
		_channels = info.channels;
		_buffer = std::vector<float>(arr, arr + size);
		_sampleRate = info.samplerate;
		sf_close(file);
		_path = fileName;
	}

	const int Sample::GetSampleRate() const
	{ 
		return _sampleRate;
	}

	const char* Sample::GetFilePath() const
	{
		return _path.c_str();
	}
}