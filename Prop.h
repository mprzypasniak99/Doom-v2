#pragma once
#include "model.h"
class Prop
{
protected:
	Model* model;
	glm::mat4 position;

public:
	Prop(Model* m, glm::mat4 pos);
	glm::mat4 getPos();
	float getPosX();
	float getPosY();
	float getPosZ();
	void draw();
};

