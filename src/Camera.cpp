#include "Camera.h"

Camera::Camera() :
	_position(glm::vec3(0.0f, 0.0f, 0.0f)),
	_worldUp(glm::vec3(0.0f, 1.0f, 0.0f)),
	_yaw(k_Yaw), _pitch(k_Pitch), _fov(k_FOV) {
	updateCameraVectors();
}
Camera::~Camera() {}

Camera::Camera(const glm::vec3 position, const glm::vec3 up, const float yaw, const float pitch) :
	_position(position),
	_worldUp(up),
	_yaw(yaw), _pitch(pitch), _fov(k_FOV) {
	updateCameraVectors();

}

Camera::Camera(const float posX, const float posY, const float posZ,
	const float upX, const float upY, const float upZ,
	const float yaw, const float pitch) :
	_position(glm::vec3(posX, posY, posZ)),
	_worldUp(glm::vec3(upX, upY, upZ)),
	_yaw(yaw), _pitch(pitch), _fov(k_FOV) {
	updateCameraVectors();

}

glm::mat4 Camera::GetViewMatrix() const {
	return LookAt();
}

glm::mat4 Camera::LookAt() const {
	return glm::lookAt(_position, _position + _front, _up);
}

float Camera::GetFOV() const {
	return _fov;
}

glm::vec3 Camera::GetPosition() const {
	return _position;
}

void Camera::updateCameraVectors() {
	glm::vec3 front;
	front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	front.y = sin(glm::radians(_pitch));
	front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	_front = glm::normalize(front);

	_right = glm::normalize(glm::cross(front, _worldUp));
	_up = glm::normalize(glm::cross(_right, _front));
}


void Camera::HandleKeyboard(const Movement direction, const float deltaTime) {
	float velocity = k_Speed * deltaTime;
	float prev_y = _position.y;
	switch (direction) {
	case Movement::Forward:
		_position += _front * velocity; break;
	case Movement::Backward:
		_position -= _front * velocity; break;
	case Movement::Left:
		_position -= _right * velocity; break;
	case Movement::Right:
		_position += _right * velocity; break;
	}

	if (!_flying) _position.y = prev_y;
}

void Camera::handleMouseMovement(const float xoffset, const float yoffset,
	const bool constrainPitch) {
	float xoff = xoffset * k_Sensitivity;
	float yoff = yoffset * k_Sensitivity;

	_yaw += xoff;
	_pitch += yoff;

	if (constrainPitch) {
		if (_pitch > 89.9f) _pitch = 89.9f;
		if (_pitch < -89.9f) _pitch = -89.9f;
	}
	updateCameraVectors();

}

void Camera::handleMouseScroll(const float yoffset) {
	if (_fov >= 1.0f && _fov <= 45.0f) _fov -= yoffset;
	if (_fov <=- 1.0f) _fov = 1.0f;
	if (_fov >= 45.0f) _fov = 45.0f;
}