#pragma once

#include "test.h"
#include "Renderer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

#include <memory>

namespace test {
	class TestTexture2d : public Test
	{
	public:
		TestTexture2d();
		~TestTexture2d();

		void OnUpdate(float deltaTime);
		void OnRender();
		void OnImGuiRender();
	private:
		std::unique_ptr<VertexArray> va;
		std::unique_ptr<VertexBuffer> vb;
		std::unique_ptr<IndexBuffer> ib;
		std::unique_ptr<Shader> shader;
		std::unique_ptr<Texture> texture;

		glm::mat4 proj, view;

		glm::vec3 translationA, translationB;
		float rotation;
	};

}