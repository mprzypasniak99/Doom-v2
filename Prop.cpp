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