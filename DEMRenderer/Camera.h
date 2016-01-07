#pragma once

#include <iostream>

#include <glm.hpp>
#include <gtx\transform.hpp>

using namespace glm;
using namespace std;

class Camera
{
public:
	explicit Camera(vec3& pos, float fov, float aspect, float zNear, float zFar);
	virtual ~Camera();

	mat4 getViewProjection() const;

	void moveRight(float x);
	void moveUp(float y);
	void moveForward(float z);
	void zoomWith(float value);

private:
	mat4 m_perspective;
	vec3 m_position;
	vec3 m_forward;
	vec3 m_up;
};

