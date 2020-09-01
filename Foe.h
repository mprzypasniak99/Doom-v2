#pragma once
#include "Prop.h"
#include "Projectile.h"
#include <chrono>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>
class Foe : public Prop
{
private:
	std::vector<glm::vec4> route;
	int count;
	std::chrono::time_point<std::chrono::system_clock> lastTime;
	std::chrono::time_point<std::chrono::system_clock> lastShoot;
	float speed;
	Model* bullet;

public:
	Foe(Model* m, glm::mat4 pos, float s, Model* b) : Prop(m, pos)
	{
		count = 0;
		lastTime = std::chrono::system_clock::now();
		lastShoot = lastTime;
		speed = s;
		bullet = b;
	}

	void addRoutePoint(glm::vec4 point);

	void updatePos(glm::mat4 V);

	void shoot(std::vector<Projectile*>* bulletTab, glm::vec4 playerPos);
};

