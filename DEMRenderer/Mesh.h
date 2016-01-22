#pragma once

#include <vector>

#include <GL\glew.h>
#include <glm.hpp>

class Mesh
{
public:
    Mesh(glm::vec3* vertices, size_t size, glm::vec3* normals);
    virtual ~Mesh();

    //make this class noncopyable
    Mesh(const Mesh&) = delete;
    Mesh operator=(const Mesh&) = delete;

    void DrawElevations();
    void DrawSurface();

private:
    enum {
        POSITION_VB,
        NORMAL_VB,
        INDEX_VB,
        NUMBUFFERS
    };

    size_t m_size;

    GLuint m_vertexArrayObject;
    GLuint m_vertexArrayBuffers[NUMBUFFERS];

};

