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
	void OnChangeFrameBufferSize(const int32_t width, const int32_t height);
private:
	GLFWwindow* _window;
};