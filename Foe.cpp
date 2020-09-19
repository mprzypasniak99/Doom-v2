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


void Foe::shoot(std::vector<Projectile*> * bulletTab, std::vector<std::vector<bool>>* col, ColDet* det, glm::vec4 playerPos)
{
	glm::vec4 dist = playerPos - position[3]; // computing position in eye space
	std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();

	std::chrono::duration<float> elapsedTime = now - lastShoot;

	if (glm::length(dist) < 20 && elapsedTime.count() >= 2.f)
	{
		lastShoot = now;

		glm::vec4 dir = glm::normalize(dist);
		
		glm::mat4 pos = position;

		float angleY = glm::orientedAngle(glm::normalize(glm::vec2(pos[2].x, pos[2].z)), glm::normalize(glm::vec2(dir.x, dir.z)));

		// angles by which projectile will be rotated to start facing the camera

		pos = glm::rotate(pos, -angleY, glm::vec3(0.f, 1.f, 0.f));

		float angleX = glm::orientedAngle(glm::normalize(glm::vec2(pos[2].y, pos[2].z)), glm::normalize(glm::vec2(dir.y, dir.z)));

		pos = glm::rotate(pos, -angleX, glm::vec3(1.f, 0.f, 0.f));

		pos = glm::translate(pos, 10.f * glm::vec3(glm::normalize(glm::transpose(pos) * dir)));

		Projectile* tmp = new Projectile(bullet, pos, dir, 2.f, 5.f);

		bulletTab->push_back(tmp);

		det->addvModel(tmp->getPosX(), tmp->getPosX(), tmp->getPosX(), 0.249151);
		(*col)[0].push_back(false);
		(*col)[1].push_back(false);
	}
}