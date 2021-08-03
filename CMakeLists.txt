cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_FLAGS_DEBUG_INIT "-Wall")

set(CMAKE_CONFIGURATION_TYPES "Debug" CACHE STRING "" FORCE)

project(AudioFramework)

link_libraries(sndfile portaudio)

add_executable(	AudioFramework 
				main.cpp 
				Core/Engine.cpp 
				Core/Log.cpp
				Core/Sample.cpp
				Playable/Playable.cpp
				Playable/Sampler.cpp
				Playable/Instruments/Instrument.cpp
				Playable/Instruments/Envelope.cpp
				Playable/Instruments/GranularSynth.cpp
				)


target_include_directories(AudioFramework PUBLIC "${PROJECT_BINARY_DIR}")

add_custom_command(TARGET AudioFramework PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E create_symlink
        ${CMAKE_SOURCE_DIR}/Res $<TARGET_FILE_DIR:AudioFramework>/Res)
