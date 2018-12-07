#include "Window.h"



Camera _camera(glm::vec3(0.0f, 0.0f, 3.0f));

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
	//glfwSetCursorPosCallback(_window, OnMouse);

	//glfwSetFramebufferSizeCallback(_window, OnChangeFrameBufferSize);

}


Window::Window(const int widht, const int height, Camera camera) {
	Window::Window(widht, height);
	_camera = camera;

}

GLFWwindow* Window::GetWindow() {
	return _window;
}
void Window::HandlerInput(const double deltaTime) {
	if (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS) {
		_camera.HandleKeyboard(Camera::Movement::Forward, deltaTime);
	}
	if (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS) {
		_camera.HandleKeyboard(Camera::Movement::Backward, deltaTime);
	}
	if (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS) {
		_camera.HandleKeyboard(Camera::Movement::Left, deltaTime);
	}
	if (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS) {
		_camera.HandleKeyboard(Camera::Movement::Right, deltaTime);
	}
	Window::HandlerInput();
}


void Window::HandlerInput() {
	//Si pulsamos la tecla ESC cerramos la ventana
	if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(_window, true);
	}

	if (glfwGetKey(_window, GLFW_KEY_0) == GLFW_PRESS) {
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

void Window::OnScroll(double xoffset, double yoffset) {
	_camera.handleMouseScroll(yoffset);
}

void Window::OnMouse(GLFWwindow* window, double xpos, double ypos) {
	if (_firstMouse) {
		_firstMouse = false;
		_lastX = xpos;
		_lastY = ypos;
	}

	_xoffset = xpos - _lastX;
	_yoffset = ypos - _lastY;
	_lastX = xpos;
	_lastY = ypos;
	_camera.handleMouseMovement(_xoffset, _yoffset);
}

bool Window::GetButtonMoreShiny() {
	return _buttonMoreShiny;
}
bool Window::GetButtonLessShiny() {
	return _buttonLessShiny;
}

//Window::OnScroll(double xoffset, double yoffset)
//{
//}
//
//void Window::OnChangeFrameBufferSize(GLFWwindow* window, const int32_t width, const int32_t height) {
//	//redimension de pantalla 
//	//Cambio de clip scene a view scene
//	glViewport(0, 0, width, height);
//}