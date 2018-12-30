#pragma once
#ifndef CAMERA_H     
#define  CAMERA_H

#include <glm/glm.hpp>
#include <glm/vec4.hpp>
#define GLFW_INCLUDE_NONE

#include<GLFW/glfw3.h>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_precision.hpp>

//Default Camera Values
const float k_Yaw = -90.0f;
const float k_Pitch = 0.0f;
const float k_Speed = 2.5f;
const float k_Sensitivity = 0.1f;
const float k_FOV = 45.0f;
class Camera {
public:
	enum class Movement { //Possible options for camera movement
		Forward = 0,
		Backward = 1,
		Left = 2,
		Right = 3,
	};
	 Camera(); //Empty Constructor
	~Camera(); //Destructor

	//Constructor with Vectors
	Camera(const glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
		const glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
		const float yaw = k_Yaw, const float pitch = k_Pitch);
	//Constructor with Scalars
	Camera(const float posX, const float posY, const float posZ,
		const float upX, const float upY, const float upZ,
		const float yaw, const float pitch);

	//returns current view matrix
	glm::mat4 GetViewMatrix() const;

	//returns current view matrix
	glm::mat4 GetViewMatrixOwnCalculate() const;

	//returns the fov
	float GetFOV() const;
	//returns position
	glm::vec3 GetPosition() const;

	//Process input from keyboard
	void HandleKeyboard(const Movement direction, const float deltaTime);
	//Process mouse movement
	void handleMouseMovement(const float xoffset, const float yoffset,
		const bool constrainPitch = true);
	//Process mouse scroll
	void handleMouseScroll(const float yoffset);

	void Camera::SetFlying(bool fly);

private:
	void updateCameraVectors(); //Calculates front vector

	glm::vec3 _position, _front, _up, _right, _worldUp; // Camera Attributes
	float _yaw, _pitch; // Euler Angles
	float _fov; // Camera options
	bool _flying = true; // Camera options

	//calculate the lookAt View matrix
	glm::mat4 LookAt() const;
	//calculate the lookAt View matrix 

	glm::mat4 LookAtOwnCalculate() const;
};
#endif