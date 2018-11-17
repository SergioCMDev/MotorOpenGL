#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<iostream>
#include<cstdint>
#include<stdio.h>
using namespace std;

#pragma region Cabezeras

void Render(GLfloat R, GLfloat G, GLfloat B, GLfloat A);
void Render();
void HandlerInput(GLFWwindow* window);
void OnChangeFrameBufferSize(GLFWwindow* window, const int32_t width, const int32_t height);
#pragma endregion


//Metodos
void Render(GLfloat R, GLfloat G, GLfloat B, GLfloat A) {
	//Renderizamos la pantalla con un color basandonos en el esquema RGBA(transparencia)
	glClearColor(R, G, B, A);
	glClear(GL_COLOR_BUFFER_BIT);
}
void Render() {
	//Renderizamos la pantalla con un color basandonos en el esquema RGBA(transparencia)
	Render(0.0f, 0.0f, 1.0f, 0.0f);
}
void HandlerInput(GLFWwindow* window) {
	//Si pulsamos la tecla ESC cerramos la ventana
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}



void OnChangeFrameBufferSize(GLFWwindow* window, const int32_t width, const int32_t height) {
	//redimension de pantalla 
	//Cambio de clip scene a view scene
	glViewport(0, 0, width, height);
}
int main(int argc, char* argv[]) {
	if (!glfwInit()) {
		cout << "Error initializing GLFW" << endl;
		glfwTerminate();
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Window 1", NULL, NULL);
	if (!window) {
		cout << "Error creating window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Error initializing GLAD" << endl;
		return -1;
	}

	//cuando la ventana cambie de tamaño
	glfwSetFramebufferSizeCallback(window, OnChangeFrameBufferSize);

	//Bucle inicial donde se realiza toda la accion del motor
	while (!glfwWindowShouldClose(window)) {
		HandlerInput(window);
		Render(1.0f, 0.0f, 0.0f, 1.0f);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}