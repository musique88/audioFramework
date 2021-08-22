#include "Engine.hpp"
#include <sstream>
#include <iostream>
#include "Log.hpp"
#include <imgui.h>
#include "../Playable/Instruments/GranularSynth.hpp"

#define DEBUG_SECONDS 10

namespace MSQ
{
	Engine* Engine::_instance = nullptr;

	Engine::Engine()
	{
		int err = Pa_Initialize();
		if (err != paNoError)
			Log::Instance()->Err(Pa_GetErrorText(err));
		_bufferSize = 512;
		_sampleRate = 0;

		_activeInstrument = -1;
		_activeOctave = 4;
		for (int i = 0; i < 128; i++)
			_notes[i] = 0;
		
	}

	void Engine::SetBufferSize(int size)
	{
		_bufferSize = size;
	}

	const int Engine::GetBufferSize() const
	{
		return _bufferSize;
	}

	const int Engine::GetSampleRate() const 
	{
		return _sampleRate;
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

	const std::vector<Sample*>& Engine::GetSamples() const
	{
		return _samples;
	}

	void Engine::OpenStream(int inIndex, int outIndex, int outChannels, int sampleRate, int bufferLength)
	{
		_sampleRate = sampleRate;
		PaStreamParameters inR;
		PaStreamParameters* in = &inR;
		if (inIndex > 0)
		{
			const PaDeviceInfo *inDeviceInfo = Pa_GetDeviceInfo(inIndex);
			in->channelCount = inDeviceInfo->maxOutputChannels;
			in->device = inIndex;
			in->hostApiSpecificStreamInfo = NULL;
			in->sampleFormat = paFloat32;
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
			out->sampleFormat = paFloat32;
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

		_sampleData = new std::vector<float>(sampleRate * DEBUG_SECONDS * outChannels);
		_buffer = new float[_bufferSize * _outputChannels];
	}

	int Engine::StreamCallback(const void* inputData, void* outputBuffer,
					unsigned long framesPerBuffer,
					const PaStreamCallbackTimeInfo* timeInfo,
					PaStreamCallbackFlags statusFlag,
					void* userData)
	{
		Engine* engine = (Engine*) userData;
		float* out = (float*)outputBuffer;
		int framesToFill = framesPerBuffer * engine->_outputChannels;
		for (int i = 0; i < framesToFill; i++)
			engine->_buffer[i] = 0;
		for(Playable* p : engine->_instruments)
		{
			if (p->GetActive())
			{
				p->Play(framesPerBuffer);
				for (int i = 0; i < framesToFill; i ++)
					engine->_buffer[i] += p->GetBuffer()[i] * 2;
			}
		}
		for(int i = 0; i < framesToFill; i++)
			out[i] = engine->_buffer[i];

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
		info.frames = DEBUG_SECONDS * _sampleRate;
		info.samplerate = _sampleRate;
		info.channels = _outputChannels;
		info.seekable = false;
		SNDFILE* sndfile = sf_open("debug.wav", SFM_WRITE, &info);
		if (int err = sf_error(sndfile))
			Log::Instance()->Err(sf_error_number(err));
		sf_write_float(sndfile, &_sampleData->at(0), _sampleData->size());
		if (int err = sf_error(sndfile))
			Log::Instance()->Err(sf_error_number(err));
		sf_close(sndfile);
		if (int err = sf_error(sndfile))
			Log::Instance()->Err(sf_error_number(err));
	}

	void Engine::AddInstrument(Instrument* p)
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

	void Engine::AddSample(const char* s)
	{
		Sample* sa = new Sample(s);
		_samples.emplace_back(sa);
	}

	void Engine::Render()
	{
		ImGui::PushID("Engine");
		ImGui::Begin("Loaded Samples");
		for (int i = 0; i < _samples.size(); i++)
		{
			ImGui::Text("%d :: %s", i,_samples[i]->GetFilePath());
		}
		ImGui::End();
		ImGui::Begin("Engine", &_on, ImGuiWindowFlags_MenuBar);
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Info"))
			{
				if (ImGui::MenuItem("Show Extra Info")) 
				{ 

				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Add"))
			{
				if (ImGui::MenuItem("GranularSynth")) 
				{
					AddInstrument(new GranularSynth());
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		ImGui::PlotLines("Buffer",_buffer, _bufferSize * _outputChannels);
		ImGui::Text("%d",_instruments.size());

		ImGui::BeginChild("Scrolling");
		for (int i = 0; i < _instruments.size(); i++)
		{
			ImGui::PushID(i);
			ImGui::Text("%d :: %s", i,_instruments[i]->GetName().c_str());
			ImGui::SameLine();
			ImGui::Checkbox("Active",&_instruments[i]->GetActive());
			ImGui::SameLine();
			if (_activeInstrument == i)
				ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.4f, 1.f, 1.f));
			else 
				ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.6f, 1.f, 1.f));
			if (ImGui::SmallButton("A"))
				_activeInstrument = i;
			ImGui::PopStyleColor();
			ImGui::SameLine();
			float a[_instruments[i]->GetBufferSize()];
			const float* ibuff = _instruments[i]->GetBuffer();
			const int maxSize = _instruments[i]->GetBufferSize();
			for (int j = 0; j < maxSize; j++)
				a[j] = (float)ibuff[j];
			ImGui::PlotLines("", a, maxSize);
			ImGui::PopID();
		}
		ImGui::EndChild();

		ImGui::End();

		ImGui::Begin("Piano Roll", nullptr, ImGuiWindowFlags_MenuBar);
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Options"))
			{
				if (ImGui::MenuItem("Octave Up")) 
				{ 
					_activeOctave += 1;
				}
				if (ImGui::MenuItem("Octave Down")) 
				{ 
					_activeOctave -= 1;
				}
				ImGui::MenuItem(std::to_string(_activeOctave).c_str());
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		for(int i = 0; i < 12; i++)
		{
			unsigned char activeNote = i + _activeOctave * 12;
			if(i == 1 || i == 3 || i == 6 || i == 8 || i == 10)
			{
				if(_notes[activeNote])
					ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.0f, 0.0f, 0.4f));
            	else
					ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.0f, 0.0f, 0.1f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.0f, 0.0f, 0.2f));
            	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.0f, 0.0f, 0.3f));
			}
			else
			{
				if(_notes[activeNote])
					ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.0f, 0.0f, 0.8f));
            	else
					ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.0f, 0.0f, 0.5f));
            	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.0f, 0.0f, 0.6f));
            	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.0f, 0.0f, 0.7f));
			}

			
			if(ImGui::Button(std::to_string(activeNote).c_str()) && _activeInstrument >= 0)
			{
				if(_notes[activeNote])
				{
					_notes[activeNote] = false;
						_instruments[_activeInstrument]->NoteOff(activeNote,128);
				}
				else
				{
					_notes[activeNote] = true;
						_instruments[_activeInstrument]->NoteOn(activeNote,128);
				}
			}
			ImGui::PopStyleColor(3);
			ImGui::SameLine();
		}

		ImGui::End();

		ImGui::PopID();
		for (Playable* p : _instruments)
			p->Render();
	}
}