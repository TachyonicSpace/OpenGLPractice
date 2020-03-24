#include "VertexBuffer.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    GLCall(glGenBuffers(1, &m_RendererID));
    //binds the buffer to the current scope
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    //specifies the type, how big the array is in bytes, points to the data, and hints at openGL how the data will be used
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RendererID))
}

void VertexBuffer::bind() const
{
    //binds the buffer to the current scope
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::unBind() const
{
    //removes the buffer from the current scope
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
