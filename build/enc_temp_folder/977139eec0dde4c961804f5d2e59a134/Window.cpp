#include "Window.h"


Window::Window()
{
	//Window::Window(800, 600);
}

Window::Window(const int widht, const int height) {
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	_window = glfwCreateWindow(widht, height, "Window 1", NULL, NULL);
	if (!_window) {
		cout << "Error creating window" << endl;
		glfwTerminate();
		//return -1;
	}
	glfwMakeContextCurrent(_window);
	//glfwSetFramebufferSizeCallback(_window, OnChangeFrameBufferSize);

}

GLFWwindow* Window::GetWindow() {
	return _window;
}

void Window::HandlerInput() {
	//Si pulsamos la tecla ESC cerramos la ventana
	if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(_window, true);
	}

	if (glfwGetKey(_window, GLFW_KEY_0) == GLFW_PRESS) {
		printf("PRESS");
		_buttonMoreShiny = true;
	}
	else
	if (glfwGetKey(_window, GLFW_KEY_0) == GLFW_RELEASE) {
		_buttonMoreShiny = false;
	}

	if (glfwGetKey(_window, GLFW_KEY_1) == GLFW_PRESS) {
		_buttonLessShiny = true;
	}
	else
	if (glfwGetKey(_window, GLFW_KEY_1) == GLFW_RELEASE) {
		_buttonLessShiny = false;
	}

}
bool Window::GetButtonMoreShiny() {
	return _buttonMoreShiny;
}
bool Window::GetButtonLessShiny() {
	return _buttonLessShiny;
}

void Window::OnChangeFrameBufferSize(GLFWwindow* window, const int32_t width, const int32_t height) {
	//redimension de pantalla 
	//Cambio de clip scene a view scene
	glViewport(0, 0, width, height);
}