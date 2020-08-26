#pragma once
#include "Prop.h"
#include <chrono>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>
class Projectile : public Prop
{
private:
	glm::vec4 direction;
	float speed;
	std::chrono::time_point<std::chrono::system_clock> lastTime; // last time position was updated
	std::chrono::time_point<std::chrono::system_clock> startTime; // time at which projectile was created
	float maxTime; // max time projectile can exist
public:
	Projectile(Model* m, glm::mat4 pos, glm::vec4 dir, float s, float lifeTime) : Prop(m, pos)
	{
		direction = dir;
		speed = s;
		startTime = std::chrono::system_clock::now();
		lastTime = startTime;
		maxTime = lifeTime;
	}
	bool updatePos(glm::mat4 V);
};

