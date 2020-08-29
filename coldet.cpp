#include "coldet.h"
#include <math.h>

ColDet::ColDet(int model_count)
{
	hitboxes_list = new Hitbox[model_count];
	next_free = 0;
}

void ColDet::addModel(float* x, float* y, float* z)
{
	Base tmp;
	tmp.point = Point(x, y, z);
	hitboxes_list[next_free].hitbox = tmp;
	hitboxes_list[next_free].type = 1;
	next_free++;
}

void ColDet::addModel(float* x, float* y, float* z, float radius)
{
	Base tmp;
	tmp.sphere = Sphere(x, y, z, radius);
	hitboxes_list[next_free].hitbox = tmp;
	hitboxes_list[next_free].type = 2;
	next_free++;
}

void ColDet::addModel(float* x, float* y, float* z, float x_shift, float y_shift, float z_shift)
{
	Base tmp;
	tmp.cuboid = Cuboid(x, y, z, x_shift, y_shift, z_shift);
	hitboxes_list[next_free].hitbox = tmp;
	hitboxes_list[next_free].type = 3;
	next_free++;
}

bool ColDet::detector()
{
	for (int i = 0; i < next_free - 1; i++)
	{
		for (int j = i + 1; i < next_free; i++)
		{
			switch (hitboxes_list[i].type)
			{
			case 1:
				switch (hitboxes_list[j].type)
				{
				case 2:
					return sqrt(pow((*(hitboxes_list[j].hitbox.sphere.Middle.X) - *(hitboxes_list[i].hitbox.point.X)), 2) + pow((*(hitboxes_list[j].hitbox.sphere.Middle.Y) - *(hitboxes_list[i].hitbox.point.Y)), 2) + pow((*(hitboxes_list[j].hitbox.sphere.Middle.Z) - *(hitboxes_list[i].hitbox.point.Z)), 2)) <= hitboxes_list[j].hitbox.sphere.Radius;
					break;
				case 3:
					return false;
					break;
				}
				break;
			case 2:
				switch (hitboxes_list[j].type)
				{
				case 1:
					return sqrt(pow((*(hitboxes_list[i].hitbox.sphere.Middle.X) - *(hitboxes_list[j].hitbox.point.X)), 2) + pow((*(hitboxes_list[i].hitbox.sphere.Middle.Y) - *(hitboxes_list[j].hitbox.point.Y)), 2) + pow((*(hitboxes_list[i].hitbox.sphere.Middle.Z) - *(hitboxes_list[j].hitbox.point.Z)), 2)) <= hitboxes_list[i].hitbox.sphere.Radius;
					break;
				case 2:
					return sqrt(pow((*(hitboxes_list[i].hitbox.sphere.Middle.X) - *(hitboxes_list[j].hitbox.sphere.Middle.X)), 2) + pow((*(hitboxes_list[i].hitbox.sphere.Middle.Y) - *(hitboxes_list[j].hitbox.sphere.Middle.Y)), 2) + pow((*(hitboxes_list[i].hitbox.sphere.Middle.Z) - *(hitboxes_list[j].hitbox.sphere.Middle.Z)), 2)) <= hitboxes_list[i].hitbox.sphere.Radius + hitboxes_list[j].hitbox.sphere.Radius;
					break;
				case 3:
					return false;
					break;
				}
				break;
			case 3:
				switch (hitboxes_list[j].type)
				{
				case 1:
					break;
				case 2:
					break;
				case 3:
					return false;
					break;
				}
			}
		}
	}
}

float*** ColDet::triangles(Cuboid model)
{
	float triangles[12][3][3];
	float vertices[8][3];
	for (int i = 0; i < 8; i++)
	{
		vertices[i][0] = *(model.one_vertex.X) + (i % 2) * model.X_shift;
		vertices[i][1] = *(model.one_vertex.Y) + ((i / 2) % 2) * model.Y_shift;
		vertices[i][2] = *(model.one_vertex.Z) + (i / 4) * model.Z_shift;
	}
	//X min
	triangles[0][0][0] = vertices[0][0];
	triangles[0][0][1] = vertices[0][1];
	triangles[0][0][2] = vertices[0][2];
	triangles[0][1][0] = vertices[4][0];
	triangles[0][1][1] = vertices[4][1];
	triangles[0][1][2] = vertices[4][2];
	triangles[0][2][0] = vertices[6][0];
	triangles[0][2][1] = vertices[6][1];
	triangles[0][2][2] = vertices[6][2];
	
	triangles[1][0][0] = vertices[0][0];
	triangles[1][0][1] = vertices[0][1];
	triangles[1][0][2] = vertices[0][2];
	triangles[1][1][0] = vertices[4][0];
	triangles[1][1][1] = vertices[4][1];
	triangles[1][1][2] = vertices[4][2];
	triangles[1][2][0] = vertices[2][0];
	triangles[1][2][1] = vertices[2][1];
	triangles[1][2][2] = vertices[2][2];

	//X max
	triangles[2][0][0] = vertices[1][0];
	triangles[2][0][1] = vertices[1][1];
	triangles[2][0][2] = vertices[1][2];
	triangles[2][1][0] = vertices[5][0];
	triangles[2][1][1] = vertices[5][1];
	triangles[2][1][2] = vertices[5][2];
	triangles[2][2][0] = vertices[7][0];
	triangles[2][2][1] = vertices[7][1];
	triangles[2][2][2] = vertices[7][2];

	triangles[3][0][0] = vertices[1][0];
	triangles[3][0][1] = vertices[1][1];
	triangles[3][0][2] = vertices[1][2];
	triangles[3][1][0] = vertices[5][0];
	triangles[3][1][1] = vertices[5][1];
	triangles[3][1][2] = vertices[5][2];
	triangles[3][2][0] = vertices[4][0];
	triangles[3][2][1] = vertices[4][1];
	triangles[3][2][2] = vertices[4][2];

	//Y min
	triangles[4][0][0] = vertices[0][0];
	triangles[4][0][1] = vertices[0][1];
	triangles[4][0][2] = vertices[0][2];
	triangles[0][1][0] = vertices[4][0];
	triangles[0][1][1] = vertices[4][1];
	triangles[0][1][2] = vertices[4][2];
	triangles[0][2][0] = vertices[6][0];
	triangles[0][2][1] = vertices[6][1];
	triangles[0][2][2] = vertices[6][2];

	triangles[1][0][0] = vertices[0][0];
	triangles[1][0][1] = vertices[0][1];
	triangles[1][0][2] = vertices[0][2];
	triangles[1][1][0] = vertices[4][0];
	triangles[1][1][1] = vertices[4][1];
	triangles[1][1][2] = vertices[4][2];
	triangles[1][2][0] = vertices[2][0];
	triangles[1][2][1] = vertices[2][1];
	triangles[1][2][2] = vertices[2][2];

	//Y max
	triangles[2][0][0] = vertices[1][0];
	triangles[2][0][1] = vertices[1][1];
	triangles[2][0][2] = vertices[1][2];
	triangles[2][1][0] = vertices[5][0];
	triangles[2][1][1] = vertices[5][1];
	triangles[2][1][2] = vertices[5][2];
	triangles[2][2][0] = vertices[7][0];
	triangles[2][2][1] = vertices[7][1];
	triangles[2][2][2] = vertices[7][2];

	triangles[3][0][0] = vertices[1][0];
	triangles[3][0][1] = vertices[1][1];
	triangles[3][0][2] = vertices[1][2];
	triangles[3][1][0] = vertices[5][0];
	triangles[3][1][1] = vertices[5][1];
	triangles[3][1][2] = vertices[5][2];
	triangles[3][2][0] = vertices[4][0];
	triangles[3][2][1] = vertices[4][1];
	triangles[3][2][2] = vertices[4][2];
}