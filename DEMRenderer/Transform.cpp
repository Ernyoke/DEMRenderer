#include "stdafx.h"
#include "Transform.h"


Transform::Transform(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale) :
    m_pos{ pos }, m_rot{ rot }, m_scale{ scale }
{
}


Transform::~Transform()
{
}

const glm::vec3& Transform::GetPos() {
	return m_pos;
}

const glm::vec3& Transform::GetRot() {
	return m_rot;
}

const glm::vec3& Transform::GetScale() {
	return m_scale;
}

void Transform::SetPos(const glm::vec3& pos) {
	m_pos = pos;
}

void Transform::SetRot(const glm::vec3& rot) {
	m_rot = rot;
}
void Transform::SetScale(const glm::vec3& scale) {
	m_scale = scale;
}
void Transform::RotateWith(glm::vec3& rot) {
	m_rot += rot;
}

const glm::mat4 Transform::GetModel() const {
    glm::mat4 posMatrix = translate(m_pos);
    glm::mat4 rotXMatrix = rotate(m_rot.x, glm::vec3(1, 0, 0));
    glm::mat4 rotYMatrix = rotate(m_rot.y, glm::vec3(0, 1, 0));
    glm::mat4 rotZMatrix = rotate(m_rot.z, glm::vec3(0, 0, 1));
    glm::mat4 scaleMatrix = scale(m_scale);

    glm::mat4 rotMatrix = rotZMatrix * rotYMatrix * rotXMatrix;

	return posMatrix * rotMatrix * scaleMatrix;
}
