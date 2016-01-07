#pragma once

#include <glm.hpp>
#include <gtx\transform.hpp>

class Transform
{
public:
	explicit Transform(const glm::vec3& pos = glm::vec3(), const glm::vec3& rot = glm::vec3(), const glm::vec3& scale = glm::vec3(1, 1, 1));
	virtual ~Transform();

	void SetPos(const glm::vec3& pos);
	void SetRot(const glm::vec3& rot);
	void SetScale(const glm::vec3& scale);

	void RotateWith(glm::vec3 &rot);

	const glm::vec3& GetPos();
	const glm::vec3& GetRot();
	const glm::vec3& GetScale();

	const glm::mat4 GetModel() const;

private:
	glm::vec3 m_pos;
	glm::vec3 m_rot;
	glm::vec3 m_scale;
};

