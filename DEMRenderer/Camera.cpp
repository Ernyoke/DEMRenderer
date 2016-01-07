#include "stdafx.h"
#include "Camera.h"


Camera::Camera(vec3& pos, float fov, float aspect, float zNear, float zFar) :
	m_position(pos)
{
	m_perspective = perspective(fov, aspect, zNear, zFar);
	m_forward = vec3(0, 0, 1);
	m_up = vec3(0, 1, 0);
}


Camera::~Camera()
{
}

mat4 Camera::getViewProjection() const {
	return m_perspective * lookAt(m_position, m_position + m_forward, m_up);
}

void Camera::moveRight(float x) {
	m_position.x += x;
	cout << x << " " << m_position.x << endl;
}

void Camera::moveUp(float y) {
	m_position.y += y;
	cout << y << " " << m_position.x << endl;
}

void Camera::moveForward(float z) {
	m_position.z += z;
	cout << z << " " << m_position.z << endl;
}

void Camera::zoomWith(float value) {
	m_position.z += value;
}

