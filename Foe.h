#pragma once
#include "Prop.h"
#include <chrono>
#include <glm/gtc/matrix_transform.hpp>
class Foe : public Prop
{
private:
	std::vector<glm::vec4> route;
	int count;
	std::chrono::time_point<std::chrono::system_clock> lastTime;
	float speed;

public:
	Foe(Model* m, glm::mat4 pos, float s) : Prop(m, pos)
	{
		count = 0;
		lastTime = std::chrono::system_clock::now();
		speed = s;
	}

	void addRoutePoint(glm::vec4 point);

	void updatePos(glm::mat4 V);
};

