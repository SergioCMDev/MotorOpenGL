//#include "UtilsHandlers.h"
//static UtilsHandlers* handlers;
//GLdouble _xpos, _ypos;
//
//
//
//
//void UtilsHandlers::t(double xpos, double ypos) {
//	handlers->OnMouse(xpos, ypos);
//}
//
//UtilsHandlers::UtilsHandlers(GLFWwindow* window, Camera camera) {
//	_window = window;
//	_camera = camera;
//
//	glfwSetFramebufferSizeCallback(_window, OnChangeFrameBufferSize);
//}
//
//
//void UtilsHandlers::OnMouse(double xpos, double ypos) {
//	glfwSetCursorPosCallback(_window, _xpos, _ypos);
//	if (_firstMouse) {
//		_firstMouse = false;
//		_lastX = xpos;
//		_lastY = ypos;
//	}
//
//	_xoffset = xpos - _lastX;
//	_yoffset = ypos - _lastY;
//	_lastX = xpos;
//	_lastY = ypos;
//	HandleMouseMovement(_xoffset, _yoffset);
//}
//
//void OnChangeFrameBufferSize(const int32_t width, const int32_t height) {
//	//redimension de pantalla 
////Cambio de clip scene a view scene
//	glViewport(0, 0, width, height);
//}
//
//
////Process input from keyboard
//void UtilsHandlers::HandleKeyboard(const Camera::Movement direction, const float deltaTime) {
//
//}
////Process mouse movement
//void UtilsHandlers::HandleMouseMovement(const float xoffset, const float yoffset,
//	const bool constrainPitch = true) {
//
//}
////Process mouse scroll
//void UtilsHandlers::HandleMouseScroll(const float yoffset) {
//
//}
//
//void UtilsHandlers::HandlerInput(const double deltaTime) {
//	if (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS) {
//		_camera.HandleKeyboard(Camera::Movement::Forward, deltaTime);
//	}
//	if (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS) {
//		_camera.HandleKeyboard(Camera::Movement::Backward, deltaTime);
//	}
//	if (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS) {
//		_camera.HandleKeyboard(Camera::Movement::Left, deltaTime);
//	}
//	if (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS) {
//		_camera.HandleKeyboard(Camera::Movement::Right, deltaTime);
//	}
//	UtilsHandlers::HandlerInput();
//}
//
//void UtilsHandlers::HandlerInput() {
//	//Si pulsamos la tecla ESC cerramos la ventana
//	if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
//		glfwSetWindowShouldClose(_window, true);
//	}
//
//	/*if (glfwGetKey(_window, GLFW_KEY_0) == GLFW_PRESS) {
//		_buttonMoreShiny = true;
//	}
//	else
//		if (glfwGetKey(_window, GLFW_KEY_0) == GLFW_RELEASE) {
//			_buttonMoreShiny = false;
//		}
//
//	if (glfwGetKey(_window, GLFW_KEY_1) == GLFW_PRESS) {
//		_buttonLessShiny = true;
//	}
//	else
//		if (glfwGetKey(_window, GLFW_KEY_1) == GLFW_RELEASE) {
//			_buttonLessShiny = false;
//		}*/
//}