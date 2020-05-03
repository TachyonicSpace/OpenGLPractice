#include "Renderer.h"
#include <iostream>

void GLClearErrors() {
    while (glGetError() != GL_NO_ERROR);
}

//print the errors to the screen, and then return if it ran correctly 
bool GLLogCall(const char* function, const char* file, int line) {
    while (GLenum error = glGetError()) {
        std::cout << "{OpenGL Error} (" << error << "):" << function << " " << file << ":" << line;
        return false;
    }return true;
}

void Renderer::Clear() const
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    shader.Bind();
    va.Bind();
    ib.bind();

    /*draw a triangle*/
    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}


float Renderer::height;
float Renderer::width;