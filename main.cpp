#include "Log.hpp"
#include "Sample.hpp"
#include "Sampler.hpp"
#include "Engine.hpp"
#include <iostream>
#include <sstream>

int main()
{
    MSQ::Log::instance()->SetFlags(MSQ::Log::LOG_ALL);
    MSQ::Sample s("song.flac");
    MSQ::Sampler sp(1024*2, s.GetChannels());
    std::cout << s.GetLength();
    sp.SetSample(&s);
    sp.SetSpeed(1.f);
    sp.SetPosition(8000000);
    static MSQ::Engine* engine = MSQ::Engine::Instance();
    engine->LogDevices();
    engine->AddInstrument(&sp);
    engine->OpenStream(-1,12,2,48000,1024);
    engine->StartStream();
    int ss;
    std::cin >> ss;
    MSQ::Log::instance()->Info("10 seconds has passed");
    engine->StopStream();
    return 0;
}