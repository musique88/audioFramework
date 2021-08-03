#include <sndfile.h>
#include "Sample.hpp"
#include "Log.hpp"
#include <string>

namespace MSQ
{
	Sample::Sample(const int& channels, const int* buffer, const int& bufferSize)
	:_channels(channels)
	{
		_buffer = std::vector<int>(buffer, buffer + bufferSize);
	}

	Sample::Sample(int channels, const std::vector<int> buffer)
	:_channels(channels), _buffer(buffer) {}

	const int Sample::GetChannels() const
	{
		return _channels;
	}

	const int Sample::GetLength() const
	{
		return _buffer.size()/GetChannels();
	}

	const std::vector<int>& Sample::GetArray() const
	{
		return _buffer;
	}

	Sample::Sample(const char* fileName)
	{
		struct SF_INFO info;
		SNDFILE* file = sf_open(fileName, SFM_READ, &info);
		if(int i = sf_error(file))
			Log::Instance()->Err(sf_error_number(i));
		int* arr = new int[info.frames * info.channels]; 
		sf_count_t size = sf_read_int(file, arr, info.frames * info.channels);
		if(int i = sf_error(file))
			Log::Instance()->Err(sf_error_number(i));
		_channels = info.channels;
		_buffer = std::vector<int>(arr, arr + size);
		sf_close(file);
	}
}