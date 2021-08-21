#pragma once
#include <vector>
#include <string>
#include "../Playable/Instruments/Instrument.hpp"
#include "Sample.hpp"
#include <portaudio.h>
#include <sndfile.h>
#include "Renderable.hpp"

namespace MSQ
{
	class Engine : public Renderable
	{
		PaStream* _stream;
		static Engine* _instance;
		static int StreamCallback(const void* inputData, void* outputBuffer,
					unsigned long framesPerBuffer,
					const PaStreamCallbackTimeInfo* timeInfo,
					PaStreamCallbackFlags statusFlag,
					void *userData);
		std::vector<Instrument*> _instruments;
		std::vector<Sample*> _samples;
		Engine();
		~Engine();
		int _sampleRate;
		int _outputChannels;
		int _bufferSize;
		std::vector<float>* _sampleData;
		int _writeHead;
		float* _buffer;

		unsigned char _activeOctave;
		bool _notes[128];
		char _activeInstrument;
	public:
		static Engine* Instance();
		void SetBufferSize(int bufferSize);
		void LogDevices() const;
		void OpenStream(int inIndex, int outIndex, int outChannels = 2, int sampleRate = 44100, int bufferLength = 512);
		void StartStream();
		void StopStream();
		void AddSample(const char*);
		const std::vector<Sample*>& GetSamples() const;
		const int GetSampleRate() const;
		const int GetBufferSize() const;
		void Hang(int seconds);
		void Hang();
		void AddInstrument(Instrument* p);
		void Render() override;
	};
}