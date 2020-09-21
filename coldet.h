/*
Header file defining basic structures and classes used to detect and deal with collisions.
v2.2 Borubash
*/

#pragma once
#ifndef COLDET_H
#define COLDET_H

#include <iostream>
#include <vector>

struct Point
{
	float X;
	float Y;
	float Z;
	Point()
	{
		X = 0; Y = 0; Z = 0;
	}
	Point(float x, float y, float z)
	{
		X = x; Y = y; Z = z;
	}
};

struct Sphere
{
	Point Middle;
	float Radius;
	Sphere(float x, float y, float z, float radius)
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
	Cuboid(float x, float y, float z, float shift)
	{
		one_vertex = Point(x, y, z);
		X_shift = shift;
		Y_shift = shift;
		Z_shift = shift;
	}
	Cuboid(float x, float y, float z, float x_shift, float y_shift, float z_shift)
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
	Cuboid cuboid = Cuboid(0, 0, 0, 0);
	Base() {}
	Base(Point p)
	{
		point = p;
	}
	Base(Sphere s)
	{
		sphere = s;
	}
	Base(Cuboid c)
	{
		cuboid = c;
	}
};
struct Hitbox
{
	Base hitbox;
	int type; // 1 - point, 2 - sphere, 3 - cuboid
	Hitbox() {}
	Hitbox(Base b, int t)
	{
		hitbox = b;
		type = t;
	}
};

class ColDet
{
public:
	ColDet() {}
	void triangles(Cuboid model, float triangles[12][3][3]);
	float* normal(float* A, float* B, float* C);
	float distance(Point a, Point b);
	bool planeCollision(float triangles[3][3], Sphere sphere);
	bool detector(Hitbox first, Hitbox second, float norm[4]);
	bool intersectRaySegmentSphere(float o[3], float d[3], Point so, float radius2);
};
#endif // !COLDET_H
