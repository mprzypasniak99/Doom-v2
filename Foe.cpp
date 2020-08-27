#include "Foe.h"

void Foe::addRoutePoint(glm::vec4 point)
{
	route.push_back(point);
}

void Foe::updatePos(glm::mat4 V)
{
	std::chrono::time_point<std::chrono::system_clock> now;
	glm::vec4 pos = position[3];
	// computing position of enemy in world space
	pos[3] = position[3][3];
	glm::vec4 posV = V * pos; // computing position in eye space
	
	now = std::chrono::system_clock::now();
	std::chrono::duration<float> elapsedTime = now - lastTime;
	lastTime = now;

	if (glm::length(posV) < 20 && glm::length(posV) > 5) // player needs to be in correct distance to the enemy for it to follow him
	{
		glm::vec4 mov = glm::transpose(position) * glm::transpose(V) * glm::normalize(posV * -1.f) * elapsedTime.count() * speed;
		
		float angle = glm::orientedAngle(glm::normalize(glm::vec2(mov.x, mov.z)), glm::normalize(glm::vec2(position[2][0], position[2][2])));
		// angle between enemy's facing direction and direction of it's movement

		position = glm::translate(position, glm::vec3(mov.x, mov.y, mov.z)); // move enemy

		position = glm::rotate(position, glm::clamp(glm::radians(10.f), 0.f, glm::radians(angle)), glm::vec3(0.f, 1.f, 0.f)); // rotate the enemy
	}
	else if (abs(glm::distance(pos, route[count])) < 1)
	{
		count++;
		count %= route.size();
	}
	else
	{
		glm::vec4 mov = glm::transpose(position) * glm::normalize(route[count] - pos) * elapsedTime.count() * speed;

		float angle = glm::orientedAngle(glm::normalize(glm::vec2(mov.x, mov.z)), glm::normalize(glm::vec2(position[2][0], position[2][2])));
		// angle between enemy's facing direction and direction of it's movement

		position = glm::translate(position, glm::vec3(mov.x, mov.y, mov.z));

		position = glm::rotate(position, glm::clamp(glm::radians(10.f), 0.f, glm::radians(angle)), glm::vec3(0.f, 1.f, 0.f)); // rotate the enemy
	}

}