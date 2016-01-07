#pragma once

#include <glm.hpp>
#include <gtx\transform.hpp>

using namespace glm;

class Transform
{
public:
	explicit Transform(const vec3& pos = vec3(), const vec3& rot = vec3(), const vec3& scale = vec3(1, 1, 1));
	virtual ~Transform();

	void setPos(const vec3& pos);
	void setRot(const vec3& rot);
	void setScale(const vec3& scale);

	void rotateWith(vec3 &rot);

	const vec3& getPos();
	const vec3& getRot();
	const vec3& getScale();

	const mat4 getModel() const;

private:
	vec3 m_pos;
	vec3 m_rot;
	vec3 m_scale;
};

