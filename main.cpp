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
	static MSQ::Engine* engine = MSQ::Engine::Instance();
	engine->LogDevices();
	engine->OpenStream(-1,12,2,48000,512);
	MSQ::Sample s("Res/song.flac");
	MSQ::GranularSynth sp;
	sp.SetSample(&s);
	engine->AddInstrument(&sp);
	engine->StartStream();
	sp.SetSpeed(0.f);
	sp.SetPosition(400000);
	sp.NoteOn(30,30);
	engine->Hang(100);
	engine->StopStream();
	return 0;
}