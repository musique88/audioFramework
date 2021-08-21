#include "Core/Log.hpp"
#include "Core/Sample.hpp"
#include "Playable/Sampler.hpp"
#include "Core/Engine.hpp"
#include "Playable/Instruments/GranularSynth.hpp"
#include <iostream>
#include <sstream>
#include <imgui.h>
#include "Req/imgui_impl_glfw.h"
#include "Req/imgui_impl_opengl3.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>


int main()
{
	srand(0);
	if (!glfwInit())
		return 1;
	const char *glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	GLFWwindow *window = glfwCreateWindow(1280, 720, "Dear ImGui - Conan", NULL, NULL);
	if (window == NULL)
		return 1;
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync

	bool err = glewInit() != GLEW_OK;
	if (err)
	{
		fprintf(stderr, "Failed to initialize OpenGL loader!\n");
		return 1;
	}

	int screen_width, screen_height;
	glfwGetFramebufferSize(window, &screen_width, &screen_height);
	glViewport(0, 0, screen_width, screen_height);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	MSQ::Log::Instance()->SetFlags(MSQ::Log::LOG_ALL);
	static MSQ::Engine* engine = MSQ::Engine::Instance();
	engine->LogDevices();
	engine->OpenStream(-1,12,2,48000,512);

	engine->StartStream();

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClearColor(0.f, 0.f, 0.f, 1.00f);
		glClear(GL_COLOR_BUFFER_BIT);

		// feed inputs to dear imgui, start new frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		engine->Render();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
	}
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
	engine->StopStream();
	return 0;
}
