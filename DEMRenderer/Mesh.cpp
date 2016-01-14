#include "stdafx.h"
#include "Mesh.h"


Mesh::Mesh(glm::vec3 *vertices, size_t size) :
    m_size{ size }
{

	glGenVertexArrays(1, &m_vertexArrayObject);
	glBindVertexArray(m_vertexArrayObject);

	glGenBuffers(NUMBUFFERS, m_vertexArrayBuffers);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]);
	glBufferData(GL_ARRAY_BUFFER, m_size * sizeof(glm::vec3), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);

}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &m_vertexArrayObject);
}

void Mesh::DrawElevations() {
	glBindVertexArray(m_vertexArrayObject);
	glDrawArrays(GL_TRIANGLES, 0, m_size);
	glBindVertexArray(0);
}

void Mesh::DrawSurface() {
}
