#include "camera.hpp"

void Camera::UpdateCam(bool tab[4]) //trzeba przekazać do funkcji tablicę wynikową z obsługi klawiszy
{
	if (tab[up])
	{
		cPos += camSpeed * cDir;
		//cPos.y -= camSpeed * cDir.y;
	}
	if (tab[down])
	{
		cPos -= camSpeed * cDir;
		//cPos.y += camSpeed * cDir.y;
	}
	if (tab[left])
	{
		cPos -= camSpeed * cDirX;
	}
	if (tab[right])
	{
		cPos += camSpeed * cDirX;
	}
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(cPos, cPos + cDir, cUp);
}

void Camera::MouseMov(float yaw, float pitch)
{
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cDir = glm::normalize(direction);
	cDirX = glm::normalize(glm::cross(cDir, cUp));
}