#pragma once
#ifndef WALL_H
#define WALL_H

float myWallVertices[] = {
	-2.f, 0.f, -2.f, 1.f,
	-2.f, 0.f, 2.f, 1.f,
	2.f, 0.f, 2.f, 1.f,

	2.f, 0.f, 2.f, 1.f,
	2.f, 0.f, -2.f, 1.f,
	-2.f, 0.f, -2.f, 1.f
};

float myWallColors[] = {
	1.0f,0.0f,0.0f,1.0f,
	1.0f,0.0f,0.0f,1.0f,
	1.0f,0.0f,0.0f,1.0f,

	1.0f,0.0f,0.0f,1.0f,
	1.0f,0.0f,0.0f,1.0f,
	1.0f,0.0f,0.0f,1.0f
};

float myWallNormals[] = {
	0.f, 1.f, 0.f, 0.f,
	0.f, 1.f, 0.f, 0.f,
	0.f, 1.f, 0.f, 0.f,

	0.f, 1.f, 0.f, 0.f,
	0.f, 1.f, 0.f, 0.f,
	0.f, 1.f, 0.f, 0.f,
};

float myWallTexCoords[] = {
	0.f, 0.f, 1.f, 0.f,
	0.f, 1.f, 1.f, 0.f,
	0.f, 1.f, 1.f, 1.f,

};

#endif // !WALL_H

