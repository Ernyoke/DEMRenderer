#include "stdafx.h"
#include "Camera.h"


Camera::Camera(glm::vec3& pos, float fov, float aspect, float zNear, float zFar) :
	m_position(pos)
{
	m_perspective = glm::perspective(fov, aspect, zNear, zFar);
	m_forward = glm::vec3(0, 0, 1);
	m_up = glm::vec3(0, 1, 0);
}


Camera::~Camera()
{
}

glm::mat4 Camera::GetViewProjection() const {
	return m_perspective * glm::lookAt(m_position, m_position + m_forward, m_up);
}

void Camera::MoveRight(float x) {
	m_position.x += x;
	cout << x << " " << m_position.x << endl;
}

void Camera::MoveUp(float y) {
	m_position.y += y;
	cout << y << " " << m_position.x << endl;
}

void Camera::MoveForward(float z) {
	m_position.z += z;
	cout << z << " " << m_position.z << endl;
}

void Camera::ZoomWith(float value) {
	m_position.z += value;
}

