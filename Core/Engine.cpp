#include "Engine.hpp"
#include <sstream>
#include <iostream>
#include "Log.hpp"

namespace MSQ
{
	Engine* Engine::_instance = nullptr;

	Engine::Engine()
	{
		int err = Pa_Initialize();
		if (err != paNoError)
			Log::Instance()->Err(Pa_GetErrorText(err));
		_bufferSize = 512;
		_sampleData.resize(44100);
	}

	void Engine::SetBufferSize(int size)
	{
		_bufferSize = size;
	}

	int Engine::GetBufferSize()
	{
		return _bufferSize;
	}

	Engine::~Engine()
	{
		for (int i = 0; i < (int)_instruments.size(); i ++)
			delete _instruments[i];
	}

	Engine* Engine::Instance()
	{
		if (_instance == nullptr)
			_instance = new Engine();
		return _instance;
	}

	void Engine::LogDevices() const
	{
		int devices = Pa_GetDeviceCount();
		for (int i= 0; i < devices; i++)
		{
			std::stringstream s;
			const PaDeviceInfo* info = Pa_GetDeviceInfo(i);
			s << i << ": " << info->name << std::endl << 
				"Default sample rate: " << info->defaultSampleRate << 
				" IN: " << info->maxInputChannels << 
				" OUT: " << info->maxOutputChannels;
			MSQ::Log::Instance()->Info(s.str());
		}
	}


	void Engine::OpenStream(int inIndex, int outIndex, int outChannels, int sampleRate, int bufferLength)
	{
		PaStreamParameters inR;
		PaStreamParameters* in = &inR;
		if (inIndex > 0)
		{
			const PaDeviceInfo *inDeviceInfo = Pa_GetDeviceInfo(inIndex);
			in->channelCount = inDeviceInfo->maxOutputChannels;
			in->device = inIndex;
			in->hostApiSpecificStreamInfo = NULL;
			in->sampleFormat = paInt32;
			in->suggestedLatency = inDeviceInfo->defaultLowOutputLatency;
		}
		else
			in = nullptr;
		PaStreamParameters outR;
		PaStreamParameters* out = &outR;
		if (outIndex > 0)
		{
			const PaDeviceInfo *outDeviceInfo = Pa_GetDeviceInfo(outIndex);
			out->channelCount = outChannels;
			_outputChannels = outChannels;
			out->device = outIndex;
			out->hostApiSpecificStreamInfo = NULL;
			out->sampleFormat = paInt32;
			out->suggestedLatency = outDeviceInfo->defaultLowOutputLatency;
		}
		else
			out = nullptr;
		std::stringstream s;
		s << "Opening stream with " << Pa_GetDeviceInfo(outIndex)->name << " as output device with " << outChannels << " channels" << std::endl;
		s << "The selected sample rate is " << sampleRate << " and the buffer size is " << bufferLength;

		Log::Instance()->Info(s.str());
		_bufferSize = bufferLength;
		int err = Pa_OpenStream(  	&_stream,
									in,
									out,
									sampleRate,
									bufferLength,
									paNoFlag,
									StreamCallback,
									this);
		if (err != paNoError)
			Log::Instance()->Err(Pa_GetErrorText(err));
	}

	int Engine::StreamCallback(const void* inputData, void* outputBuffer,
					unsigned long framesPerBuffer,
					const PaStreamCallbackTimeInfo* timeInfo,
					PaStreamCallbackFlags statusFlag,
					void* userData)
	{
		Engine* engine = (Engine*) userData;
		int* out = (int*)outputBuffer;
		int framesToFill = framesPerBuffer * engine->_outputChannels;
		for (int i = 0; i < framesToFill; i++)
			out[i] = 0;
		for(Playable* p : engine->_instruments)
		{
			if (p->GetActive())
			{
				p->Play(framesPerBuffer);
				for (int i = 0; i < framesToFill; i ++)
					out[i] += p->GetBuffer()[i]/2;
			}
		}

//		for(int i = 0; i < framesToFill; i++)
//			out[i] /= 16;
		for (int i = 0; i < framesToFill; i ++)
			engine->_sampleData[(i + engine->_writeHead) % 44100] = out[i];
		engine->_writeHead += framesToFill;
		return paContinue;
	}

	void Engine::StartStream()
	{
		int err = Pa_StartStream(_stream);
		if (err != paNoError)
			Log::Instance()->Err(Pa_GetErrorText(err));
	}
	
	void Engine::StopStream()
	{
		int err = Pa_StopStream(_stream);
		if (err != paNoError)
			Log::Instance()->Err(Pa_GetErrorText(err));
		SF_INFO info;
		info.format = SF_FORMAT_WAV | SF_FORMAT_PCM_32;
		info.frames = 44100/2;
		info.samplerate = 44100;
		info.channels = 2;
		info.seekable = false;
		SNDFILE* sndfile = sf_open("debug.wav", SFM_WRITE, &info);
		if (int err = sf_error(sndfile))
			Log::Instance()->Err(sf_error_number(err));
		sf_write_int(sndfile, &_sampleData[0], 44100);
		if (int err = sf_error(sndfile))
			Log::Instance()->Err(sf_error_number(err));
		sf_close(sndfile);
		if (int err = sf_error(sndfile))
			Log::Instance()->Err(sf_error_number(err));
	}

	void Engine::AddInstrument(Playable* p)
	{
		_instruments.push_back(p);
	}

	void Engine::Hang(int seconds)
	{
		Pa_Sleep(seconds*1000);
	}

	void Engine::Hang()
	{
		char a;
		std::cin >> a;
	}
}