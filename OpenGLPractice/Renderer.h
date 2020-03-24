#pragma once

#include <GL/glew.h>

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