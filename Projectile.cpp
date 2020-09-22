#include "Projectile.h"

bool Projectile::updatePos(glm::mat4 V)
{
	std::chrono::time_point<std::chrono::system_clock> now;
	glm::vec4 pos = position[3];

	glm::vec4 posV = V * pos;

	now = std::chrono::system_clock::now();
	std::chrono::duration<float> elapsedTime = now - lastTime;
	std::chrono::duration<float> LifeTime = now - startTime;
	lastTime = now;

	if (LifeTime.count() >= maxTime)
	{
		return false;
	}

	glm::vec4 mov = glm::transpose(position) * glm::normalize(direction) * elapsedTime.count() * speed;

	position = glm::translate(position, glm::vec3(mov.x, mov.y, mov.z));

	return true;
}