#pragma once
#include <vector>
#include <string>
#include "../Playable/Playable.hpp"
#include <portaudio.h>
#include <sndfile.h>

namespace MSQ
{
	class Engine
	{
		PaStream* _stream;
		static Engine* _instance;
		static int StreamCallback(const void* inputData, void* outputBuffer,
					unsigned long framesPerBuffer,
					const PaStreamCallbackTimeInfo* timeInfo,
					PaStreamCallbackFlags statusFlag,
					void *userData);
		std::vector<Playable*> _instruments;
		Engine();
		~Engine();
		int _sampleRate;
		int _outputChannels;
		int _bufferSize;
		std::vector<int>* _sampleData;
		int _writeHead;
	public:
		static Engine* Instance();
		void SetBufferSize(int bufferSize);
		void LogDevices() const;
		void OpenStream(int inIndex, int outIndex, int outChannels = 2, int sampleRate = 44100, int bufferLength = 512);
		void StartStream();
		void StopStream();
		const int GetSampleRate() const;
		const int GetBufferSize() const;
		void Hang(int seconds);
		void Hang();
		void AddInstrument(Playable* p);
	};
}