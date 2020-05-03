#pragma once

#include <GL/glew.h>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

//if x is false then add a breakpoint to that line of code
#define ASSERT(x) if (!(x)) __debugbreak();
//clear errors from openGL, run x, then checks for errors
#define GLCall(x) GLClearErrors();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

//clear errors from openGL
void GLClearErrors();

//print the errors to the screen, and then return if it ran correctly
bool GLLogCall(const char* function, const char* file, int line);

class Renderer 
{
public:
    void Clear() const;
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
    static float height, width;
};