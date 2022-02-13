#pragma once

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
//#include "Character.h"

class Camera
{
public:
	Camera(glm::vec3 position, glm::vec3 target, glm::vec3 worldup);
	Camera(glm::vec3 position, float pitch, float yaw, glm::vec3 worldup);

	glm::vec3 Position;
	glm::vec3 Forward;
	glm::vec3 Right;
	glm::vec3 Up;
	glm::vec3 WorldUp;
	glm::vec3 LastPosition = glm::vec3(0.0f);
	glm::vec3 firstPersonPosition = glm::vec3(0.0f);
	glm::vec3 firstPersonTarget = glm::normalize(glm::vec3(1.0f));
	glm::vec3 firstPersonUp = glm::vec3(0.0f, 1.0f, 0.0f);
	float fov = 30.0;
	float Pitch;
	float Yaw;
	float SenseX = 0.005;
	float SenseY = 0.005;
	float SenseSpeedZ = 0.1;
	float SenseSpeedX = 0.1;
	float SenseSpeedY = 0.1;
	float SpeedZ = 0;
	float SpeedX = 0;
	float SpeedY = 0;

	glm::mat4 GetViewMatrix();
	glm::mat4 GetViewMatrixFirstPerson();
	void ProcessMouseMovement(float Delta_x, float Delta_y);
	void UpdateCameraPos();
	void ProcessMouseMovement_ThirdPersonView(float Delta_x, float Delta_y);
	void UpdateCameraPos_ThirdPersonView(glm::vec3 dispalcement);
	/*void ThirdPersonView_LookAtCharacter(Character character);
	void UpdateCameraVector_ThirdPersonView(Character character);
	void TopDownView_LookAtCharacter(Character character);
	void UpdateCameraVector_TopDownView(Character character);*/

private:
	void UpdateCameraVector();

};

