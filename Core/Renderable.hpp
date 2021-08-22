#pragma once
#include <imgui.h>
#include <string>
#include <cstdlib>

namespace MSQ
{
	class Renderable
	{
	protected:
		bool _on;
		unsigned long int _id;
	public:
		virtual void Render(){ImGui::Begin("Placeholder renderable"); ImGui::End();}
		Renderable():_on(true), _id(rand()){}
	};
}