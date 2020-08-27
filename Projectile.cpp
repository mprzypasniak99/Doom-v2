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
	/*glm::vec4 position_Z_axis_V = V * position[2];

	float angleY = glm::orientedAngle(glm::normalize(glm::vec2(posV.x, posV.z)), glm::normalize(glm::vec2(position_Z_axis_V.x, position_Z_axis_V.z)));

	float angleX = glm::orientedAngle(glm::normalize(glm::vec2(posV.y, posV.z)), glm::normalize(glm::vec2(position_Z_axis_V.y, position_Z_axis_V.z)));

	position = glm::rotate(position, -glm::radians(angleY), glm::vec3(0.f, 1.f, 0.f));
	position = glm::rotate(position, glm::radians(angleX), glm::vec3(1.f, 0.f, 0.f));*/
}