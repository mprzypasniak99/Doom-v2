#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>


class Model
{
protected:
	float* vertices;
	float* colors;
	float* normals;
	float* texCoords;
	int vertexCount;

	GLuint tex;
public:
	Model(float* v, float* c, float* n, float* tex, int count);
	Model(const char* model, const char* texture);

	bool load3dModel(const char* model);

	void setTex(GLuint t);

	void draw();
	void scale(float multiplier);

};

class DoomGuy
{
protected:
	float* vertices[2];
	float* normals[2];
	float* texCoords[2];
	int vertexCount[2];

	GLuint tex[2];
public:
	bool initDoomGuy(); //Procedura do wczytywania z .obj i tekstur
	void drawDoomGuy(); //Procedura rysuj�ca
};

GLuint readTexture(const char* filename);




