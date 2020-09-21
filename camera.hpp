#pragma once
#ifndef CAMERA_HPP_INCLUDED
#define CAMERA_HPP_INCLUDED

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


enum dir { up, down, left, right };

class Camera
{
private:
	glm::vec3 cPos; 
	glm::vec3 cDir;
	glm::vec3 cUp;
	glm::vec3 cDirX;
	float yaw; //kąt obrotu kamery wokół osi Y
	float pitch; //kąt obrotu kamery wokół osi X
	float camSpeed; //prędkość poruszania się kamery
	float angleSpeed; //prędkość obrotu kamery
public:
	Camera()
	{
		cPos = glm::vec3(0.f, 0.f, 10.f);
		cDir = glm::vec3(0.f, 0.f, -1.f);
		cUp = glm::vec3(0.f, 1.f, 0.f);
		cDirX = glm::vec3(1.f, 0.f, 0.f);
		camSpeed = 0.5f;
		angleSpeed = 1.5f;
		yaw = -90.f;
		pitch = 0.f;
	}

	void UpdateCam(bool tab[4]);
	void MouseMov(float yaw, float pitch);
	glm::mat4 GetViewMatrix();
	glm::vec3 getPos();
	glm::vec3 getDir();
	float getCamSpeed();
	void UpdatePos(float* shift);
};

#endif // !CAMERA_HPP_INCLUDED
