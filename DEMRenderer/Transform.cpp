#include "stdafx.h"
#include "Transform.h"


Transform::Transform(const vec3& pos, const vec3& rot, const vec3& scale) :
	m_pos(pos), m_rot(rot), m_scale(scale)
{
}


Transform::~Transform()
{
}

const vec3& Transform::getPos() {
	return m_pos;
}

const vec3& Transform::getRot() {
	return m_rot;
}

const vec3& Transform::getScale() {
	return m_scale;
}

void Transform::setPos(const vec3& pos) {
	m_pos = pos;
}

void Transform::setRot(const vec3& rot) {
	m_rot = rot;
}
void Transform::setScale(const vec3& scale) {
	m_scale = scale;
}
void Transform::rotateWith(vec3& rot) {
	m_rot += rot;
}

const mat4 Transform::getModel() const {
	mat4 posMatrix = translate(m_pos);
	mat4 rotXMatrix = rotate(m_rot.x, vec3(1, 0, 0));
	mat4 rotYMatrix = rotate(m_rot.y, vec3(0, 1, 0));
	mat4 rotZMatrix = rotate(m_rot.z, vec3(0, 0, 1));
	mat4 scaleMatrix = scale(m_scale);

	mat4 rotMatrix = rotZMatrix * rotYMatrix * rotXMatrix;

	return posMatrix * rotMatrix * scaleMatrix;
}
