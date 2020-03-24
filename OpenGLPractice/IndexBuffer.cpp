#include "IndexBuffer.h"
#include "Renderer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
    :m_Count(count)
{
    ASSERT(sizeof(unsigned int) == sizeof(GLuint))

    GLCall(glGenBuffers(1, &m_RendererID));
    //binds the buffer to the current scope
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
    //specifies the type, hELEMENT_ow big the array is in bytes, points to the data, and hints at openGL how the data will be used
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RendererID))
}

void IndexBuffer::bind() const
{
    //binds the buffer to the current scope
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::unBind() const
{
    //removes the buffer from the current scope
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
