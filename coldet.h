#pragma once
#ifndef COLDET_H
#define COLDET_H

#include "model.h"
#include <iostream>

struct Point
{
	float *X;
	float *Y;
	float *Z;
	Point()
	{
		X = nullptr; Y = nullptr; Z = nullptr;
	}
	Point(float* x, float* y, float* z)
	{
		X = x; Y = y; Z = z;
	}
};

struct Sphere
{
	Point Middle;
	float Radius;
	Sphere(float* x, float* y, float* z, float radius)
	{
		Middle = Point(x, y, z);
		Radius = radius;
	}
};

struct Cuboid
{
	Point one_vertex;
	float X_shift;
	float Y_shift;
	float Z_shift;
	Cuboid(float* x, float* y, float* z, float shift)
	{
		one_vertex = Point(x, y, z);
		X_shift = shift;
		Y_shift = shift;
		Z_shift = shift;
	}
	Cuboid(float* x, float* y, float* z, float x_shift, float y_shift, float z_shift)
	{
		one_vertex = Point(x, y, z);
		X_shift = x_shift;
		Y_shift = y_shift;
		Z_shift = z_shift;
	}
};

union Base
{
	Point point;
	Sphere sphere;
	Cuboid cuboid;
	Base(){}
};
struct Hitbox
{
	Base hitbox;
	int type; // 1 - point, 2 - sphere, 3 - cuboid
	Hitbox() {}
};

class ColDet
{
private:
	Hitbox* hitboxes_list;
	int next_free;
public:
	ColDet(int model_count);
	void addModel(float* x, float* y, float* z);
	void addModel(float* x, float* y, float* z, float radius);
	void addModel(float* x, float* y, float* z, float x_shift, float y_shift, float z_shift);
	float*** triangles(Cuboid model);
	float* normal(float x, float y, float z);
	float distance(Point a, Point b);
	float planeCollision();
	bool detector();
};
#endif // !COLDET_H
