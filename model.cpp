#include "model.h"
#include "lodepng.h"
#include "OBJ_Loader.h"


Model::Model(float* v, float* c, float* n, float* tex, int count)
{
	vertexCount = count;
	vertices = v;
	colors = c;
	normals = n;
	texCoords = tex;

	this->tex = NULL;
}

Model::Model(const char* model, const char* texture)
{
	tex = readTexture(texture);
	colors = NULL;
	if (load3dModel(model))
	{
		printf("Successfully loaded model %s\n", model);
	}
	else
	{
		printf("Failed to load model %s\n", model);
	}
}

bool Model::load3dModel(const char* model)
{
	objl::Loader loader;
	if (!loader.LoadFile(model))
	{
		return false;
	}

	std::vector<float> v, n, t;
	for (int i = 0; i < loader.LoadedMeshes[0].Indices.size(); i++)
	{
		int tmp = loader.LoadedMeshes[0].Indices[i];

		v.push_back(loader.LoadedMeshes[0].Vertices[tmp].Position.X);
		v.push_back(loader.LoadedMeshes[0].Vertices[tmp].Position.Y);
		v.push_back(loader.LoadedMeshes[0].Vertices[tmp].Position.Z);
		v.push_back(1.f);


		n.push_back(loader.LoadedMeshes[0].Vertices[tmp].Normal.X);
		n.push_back(loader.LoadedMeshes[0].Vertices[tmp].Normal.Y);
		n.push_back(loader.LoadedMeshes[0].Vertices[tmp].Normal.Z);
		n.push_back(0.f);

		t.push_back(loader.LoadedMeshes[0].Vertices[tmp].TextureCoordinate.X);
		t.push_back(loader.LoadedMeshes[0].Vertices[tmp].TextureCoordinate.Y);
	}
	vertices = new float[v.size()];
	normals = new float[n.size()];
	texCoords = new float[t.size()];
	vertexCount = loader.LoadedMeshes[0].Indices.size();

	for (int i = 0; i < v.size(); i++)
	{
		vertices[i] = v[i];
	}

	for (int i = 0; i < n.size(); i++)
	{
		normals[i] = n[i];
	}

	for (int i = 0; i < t.size(); i++)
	{
		texCoords[i] = t[i];
	}

	return true;
}

void Model::draw()
{
	glVertexAttribPointer(1, 4, GL_FLOAT, false, 0, vertices); //Wska? tablic? z danymi dla atrybutu vertex
	
	glVertexAttribPointer(2, 4, GL_FLOAT, false, 0, colors);

	glVertexAttribPointer(3, 4, GL_FLOAT, false, 0, normals);

	glVertexAttribPointer(4, 2, GL_FLOAT, false, 0, texCoords);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);

	glDrawArrays(GL_TRIANGLES, 0, vertexCount); //Narysuj obiekt
}

GLuint readTexture(const char* filename) {
	GLuint tex;
	glActiveTexture(GL_TEXTURE0);
	//Wczytanie do pamięci komputera
	
	std::vector<unsigned char> image; //Alokuj wektor do wczytania obrazka
	unsigned width, height; //Zmienne do których wczytamy wymiary obrazka
	//Wczytaj obrazek
	
	unsigned error = lodepng::decode(image, width, height, filename);
	//Import do pamięci karty graficznej
	glGenTextures(1, &tex); //Zainicjuj jeden uchwyt
	glBindTexture(GL_TEXTURE_2D, tex); //Uaktywnij uchwyt
	//Wczytaj obrazek do pamięci KG skojarzonej z uchwytem
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	return tex;
}

void Model::setTex(GLuint t)
{
	tex = t;
}

void Model::scale(float multiplier)
{
	for (int i = 3; i < vertexCount * 4; i += 4)
	{
		vertices[i] *= multiplier;
	}
}

float* Model::getVertices()
{
	return vertices;
}

int Model::getVertexCount()
{
	return vertexCount;
}
