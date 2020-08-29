#include "Prop.h"

Prop::Prop(Model* m, glm::mat4 pos)
{
	position = pos;
	model = m;
}

glm::mat4 Prop::getPos()
{
	return position;
}

void Prop::draw()
{
	model->draw();
}

float* Prop::getPosX()
{
	return &position[3][0];
}

float* Prop::getPosY()
{
	return &position[3][1];
}

float* Prop::getPosZ()
{
	return &position[3][2];
}