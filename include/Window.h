#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stdio.h>
#include<iostream>
#include<cstdint>
using namespace std;

class Window {
public:
	Window();
	Window(const int widht, const int height);
	GLFWwindow* GetWindow();
	void HandlerInput();
private:
	void OnChangeFrameBufferSize(GLFWwindow* window, const int32_t width, const int32_t height);
	GLFWwindow* _window;
};