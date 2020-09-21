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
	int health = 4;

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

	void updatePos(float* shift);

	//void shoot(glm::vec4 playerPos);

	void kill_this_bastard();

	bool is_bastard_alive();
	//debugging
	std::chrono::time_point<std::chrono::system_clock> lS();
	void setlS(std::chrono::time_point<std::chrono::system_clock> time);
	Model* returnBullet();

	float getSpeed();
};

