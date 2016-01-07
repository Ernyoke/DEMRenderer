#pragma once

#include "glm.hpp"

using namespace glm;

class Vertex
{
public:
	Vertex(const vec3 position);
	virtual ~Vertex();

private:
	vec3 m_position;
};

