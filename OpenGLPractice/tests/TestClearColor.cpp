#include "TestClearColor.h"
#include "Renderer.h"
#include "imgui\imgui.h"

namespace test {



	TestClearColor::TestClearColor():m_clearColor {.2f, .3f, .8f, 1.f}
	{
	}

	TestClearColor::~TestClearColor()
	{
	}

	void TestClearColor::OnUpdate(float deltaTime)
	{
	}

	void TestClearColor::OnRender()
	{
		GLCall(glClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], m_clearColor[3]));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
	}

	void TestClearColor::OnImGuiRender()
	{
		ImGui::ColorEdit4("Clear Color", m_clearColor);
	}

}