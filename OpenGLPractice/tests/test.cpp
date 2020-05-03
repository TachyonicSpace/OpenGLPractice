#include "test.h"
#include "imgui\imgui.h"

namespace test {
	TestMenu::TestMenu(Test*& currentPtr): m_currentTest(currentPtr)
	{
	}

	void TestMenu::OnImGuiRender()
	{
		for (auto& test : m_tests) {
			if (ImGui::Button(test.first.c_str())) {
				std::cout << "pressed";
				m_currentTest = test.second();
			}
		}
	}
}