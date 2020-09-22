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
	long vertexCount;

	GLuint tex;
public:
	Model(float* v, float* c, float* n, float* tex, int count);
	Model(const char* model, const char* texture);

	bool load3dModel(const char* model);

	void setTex(GLuint t);

	void draw();
	void scale(float multiplier);

	float* getVertices();
	int getVertexCount();
};

GLuint readTexture(const char* filename);




