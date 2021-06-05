#include "Log.hpp"
#include "Sample.hpp"
#include "Sampler.hpp"
#include "Engine.hpp"

int main()
{
    MSQ::Log::instance()->SetFlags(MSQ::Log::LOG_ALL);
    MSQ::Sample s("song.flac");
    MSQ::Sampler sp(1024*2, s.GetChannels());
    sp.SetSample(&s);
    MSQ::Sampler sp2(1024*2, s.GetChannels());
    sp2.SetSample(&s);
    sp2.SetPosition(100000);
    static MSQ::Engine* engine = MSQ::Engine::Instance();
    engine->LogDevices();
    engine->AddInstrument(&sp);
    engine->AddInstrument(&sp2);
    engine->OpenStream(-1,10,2,48000,1024);
    engine->StartStream();

    engine->Hang(10);
    sp.SetPosition(200000);
    MSQ::Log::instance()->Info("10 seconds has passed");
    engine->Hang(10);
    return 0;
}