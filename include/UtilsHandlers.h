//#ifndef UTILSHANDLERS_H
//#define UTILSHANDLERS_H
//
//#include<iostream>
//#include <string>
//#include <Window.h>
//#include<glad/glad.h>
//#include<GLFW/glfw3.h>
//
//#include<iostream>
//#include<cstdint>
//#include<stdio.h>
//#include<Camera.h>
//
//class UtilsHandlers {
//
//public:
//	UtilsHandlers(GLFWwindow* window, Camera camera);
//	void HandlerInput(const double deltaTime);
//	void HandlerInput();
//	//Process input from keyboard
//	void HandleKeyboard(const Camera::Movement direction, const float deltaTime);
//	//Process mouse movement
//	void HandleMouseMovement(const float xoffset, const float yoffset,
//		const bool constrainPitch = true);
//	//Process mouse scroll
//	void HandleMouseScroll(const float yoffset);
//	void t(double xpos, double ypos);
//private:
//	void OnChangeFrameBufferSize(const int32_t width, const int32_t height);
//	void OnMouse(double xpos, double ypos);
//	GLFWwindow* _window;
//	Camera _camera;
//	bool _firstMouse = false;
//	double _lastX, _lastY, _xoffset, _yoffset;
//};
//#endif