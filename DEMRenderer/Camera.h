#pragma once

#include <iostream>

#include <glm.hpp>
#include <gtx\transform.hpp>

using namespace std;

class Camera
{
public:
	explicit Camera(glm::vec3& pos, float fov, float aspect, float zNear, float zFar);
	virtual ~Camera();

    glm::mat4 GetViewProjection() const;

	void MoveRight(float x);
	void MoveUp(float y);
	void MoveForward(float z);
	void ZoomWith(float value);

private:
    glm::mat4 m_perspective;
    glm::vec3 m_position;
	glm::vec3 m_forward;
	glm::vec3 m_up;
};

