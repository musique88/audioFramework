#include "Core/Log.hpp"
#include "Core/Sample.hpp"
#include "Playable/Sampler.hpp"
#include "Core/Engine.hpp"
#include "Playable/Instruments/GranularSynth.hpp"
#include <iostream>
#include <sstream>

int main()
{
	MSQ::Log::Instance()->SetFlags(MSQ::Log::LOG_ALL);
	MSQ::Sample s("Res/song.flac");
	MSQ::Sampler sp(2);
	sp.SetSample(&s);
	static MSQ::Engine* engine = MSQ::Engine::Instance();
	engine->LogDevices();
	engine->OpenStream(-1,12,2,48000,512);
	engine->AddInstrument(&sp);
	engine->StartStream();
	engine->Hang(10);
	engine->StopStream();
	return 0;
}