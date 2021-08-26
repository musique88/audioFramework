cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_FLAGS_DEBUG_INIT "-Wall")

set(CMAKE_CONFIGURATION_TYPES "Debug" CACHE STRING "" FORCE)

project(AudioFramework)

link_libraries(sndfile portaudio glfw GLEW GL imgui)

add_executable(	AudioFramework 
				mainUi.cpp 
				Core/Engine.cpp 
				Core/Log.cpp
				Core/Sample.cpp
				Playable/Playable.cpp
				Playable/Instruments/Sampler.cpp
				Playable/Instruments/LoopingSampler.cpp
				Playable/Instruments/Instrument.cpp
				Playable/Instruments/Envelope.cpp
				Playable/Instruments/GranularSynth.cpp
				Playable/Instruments/InstrumentsUtils.cpp
				Req/imgui_impl_opengl3.cpp
				Req/imgui_impl_glfw.cpp
				)
set(CMAKE_EXE_LINKER_FLAGS "-rdynamic")
target_include_directories(AudioFramework PUBLIC "${PROJECT_BINARY_DIR}" )

add_custom_command(TARGET AudioFramework PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E create_symlink
        ${CMAKE_SOURCE_DIR}/Res $<TARGET_FILE_DIR:AudioFramework>/Res)
