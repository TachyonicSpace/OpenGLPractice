#pragma once

#include "test.h"

namespace test {
	class TestClearColor : public Test
	{
	public:
		TestClearColor();
		~TestClearColor();

		void OnUpdate(float deltaTime);
		void OnRender();
		void OnImGuiRender();
	private:
		float m_clearColor[4];
	};

}