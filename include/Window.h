#pragma once
#ifndef WINDOW_H     
#define  WINDOW_H

#include<stdio.h>
#include<iostream>
#include<cstdint>
#define GLFW_INCLUDE_NONE
#include <Camera.h>
#include<GLFW/glfw3.h>


using namespace std;

class Window {
public:
	Window();
	Window(const int widht, const int height);
	Window(const int widht, const int height, Camera camera);
	GLFWwindow* GetWindow();
	void HandlerInput();
	void HandlerInput(const double deltaTime);
	bool GetButtonMoreShiny();
	bool GetButtonLessShiny();
	void OnScroll(double xoffset, double yoffset);
	void OnMouse(GLFWwindow* window, double xpos, double ypos);
private:
	//void OnChangeFrameBufferSize(GLFWwindow* window, const int32_t width, const int32_t height);
	GLFWwindow* _window;
	bool _buttonMoreShiny = false;
	bool _buttonLessShiny = false;
	bool _firstMouse = false;
	double _lastX, _lastY, _xoffset, _yoffset;
};
#endif