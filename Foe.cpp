#include "Foe.h"

void Foe::addRoutePoint(glm::vec4 point)
{
	route.push_back(point);
}

void Foe::updatePos(glm::mat4 V)
{
	std::chrono::time_point<std::chrono::system_clock> now;
	glm::vec4 pos = position[3];//[0] * position[0] + position[3][1] * position[1] + position[3][2] * position[2];
	// computing position of enemy in world space
	pos[3] = position[3][3];
	pos = V * pos; // computing position in eye space
	
	now = std::chrono::system_clock::now();
	std::chrono::duration<float> elapsedTime = now - lastTime;
	lastTime = now;

	if (glm::length(pos) < 20 && glm::length(pos) > 5) // player needs to be in correct distance to the enemy for it to follow him
	{
		glm::vec4 mov = glm::normalize(pos * -1.f) * elapsedTime.count() * speed;
		
		position = glm::translate(position, glm::vec3(mov.x, mov.y, mov.z));
	}

	// For some reason enemy keeps on going in the same direction. Distance is correctly detected
	// it just keeps on flying in camera's primary position

}