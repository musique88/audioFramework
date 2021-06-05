#pragma once
#include <vector>

namespace MSQ
{
    class Sample
    {
        int _channels;
        std::vector<int> _buffer;
    public: 
        Sample(const int& channels, const int* buffer, const int& bufferSize);
        Sample(int channels, const std::vector<int> buffer);
        Sample(const char* file);
        const int GetChannels() const;
        const int GetLength() const;
        const std::vector<int>& GetArray() const;
    };
}