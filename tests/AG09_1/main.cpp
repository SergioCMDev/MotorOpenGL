#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<iostream>
#include<cstdint>
#include<stdio.h>

#include "Shader.h"
#include "Utils.h"
#include "Window.h"
#include "Buffer.h"
#include "Camera.h"
#include "Model.h"


Utils utils;
Camera camera(glm::vec3(-1.0f, 2.0f, 3.0f));

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

glm::vec3 pointLightPositions[] = {
	glm::vec3(0.7f, 0.2f, 2.0f),
glm::vec3(2.3f, -3.3f, -4.0f)

};

float lastFrame = 0.0f;
bool firstMouse = true;

const uint32_t screen_width = 800, screen_height = 600;
float lastX = (float)screen_width / 2.0f;
float lastY = (float)screen_height / 2.0f;
Window window;

bool _firstMouse = false;
double _lastX, _lastY, _xoffset, _yoffset;
uint32_t _elementsVertexs = 192;

float vertex[] = {  //vertices      //uvs     //normals
	  -0.5f,  -0.5f,  0.5f,       0.0f, 0.0f,     0.0f, 0.0f, 1.0f,//front
	  0.5f,  -0.5f,  0.5f,       1.0f, 0.0f,      0.0f, 0.0f, 1.0f,
	  0.5f,  0.5f,  0.5f,       1.0f, 1.0f,       0.0f, 0.0f, 1.0f,
	  -0.5f, 0.5f,  0.5f,       0.0f, 1.0f,       0.0f, 0.0f, 1.0f,

	  0.5f,  -0.5f,  0.5f,       0.0f, 0.0f,      1.0f, 0.0f, 0.0f,//right
	  0.5f,  -0.5f,  -0.5f,       1.0f, 0.0f,     1.0f, 0.0f, 0.0f,
	  0.5f,  0.5f,  -0.5f,       1.0f, 1.0f,      1.0f, 0.0f, 0.0f,
	  0.5f,  0.5f,  0.5f,       0.0f, 1.0f,       1.0f, 0.0f, 0.0f,

	  -0.5f,  -0.5f,  -0.5f,       1.0f, 0.0f,    0.0f, 0.0f, -1.0f,//back
	  -0.5f,  0.5f,  -0.5f,       1.0f, 1.0f,     0.0f, 0.0f, -1.0f,
	  0.5f,  0.5f,  -0.5f,       0.0f, 1.0f,      0.0f, 0.0f, -1.0f,
	  0.5f,  -0.5f,  -0.5f,       0.0f, 0.0f,     0.0f, 0.0f, -1.0f,

	  -0.5f,  -0.5f,  0.5f,       1.0f, 0.0f,     -1.0f, 0.0f, 0.0f,//left
	  -0.5f,  0.5f,  0.5f,       1.0f, 1.0f,      -1.0f, 0.0f, 0.0f,
	  -0.5f,  0.5f,  -0.5f,       0.0f, 1.0f,     -1.0f, 0.0f, 0.0f,
	  -0.5f,  -0.5f,  -0.5f,       0.0f, 0.0f,    -1.0f, 0.0f, 0.0f,

	  -0.5f,  -0.5f,  0.5f,       0.0f, 1.0f,     0.0f, -1.0f, 0.0f,//bottom
	  -0.5f,  -0.5f,  -0.5f,       0.0f, 0.0f,    0.0f, -1.0f, 0.0f,
	  0.5f,  -0.5f,  -0.5f,       1.0f, 0.0f,     0.0f, -1.0f, 0.0f,
	  0.5f,  -0.5f,  0.5f,       1.0f, 1.0f,      0.0f, -1.0f, 0.0f,

	  -0.5f,  0.5f,  0.5f,       0.0f, 0.0f,      0.0f, 1.0f, 0.0f,//top
	  0.5f,  0.5f,  0.5f,       1.0f, 0.0f,       0.0f, 1.0f, 0.0f,
	  0.5f,  0.5f,  -0.5f,       1.0f, 1.0f,      0.0f, 1.0f, 0.0f,
	  -0.5f,  0.5f,  -0.5f,       0.0f, 1.0f,     0.0f, 1.0f, 0.0f };


uint32_t elementsIndexes = 36;

uint32_t indexes[]{
	0, 1, 2, 0, 2, 3 //Front
	,4, 5, 6, 4, 6, 7 //Right
	,8, 9, 10, 8, 10, 11 //Back
	,12, 13, 14, 12, 14, 15 //Left
	,16, 17, 18, 16, 18, 19 //Bottom
	,20, 21, 22, 20, 22, 23 //Top
};
glm::vec3 cubePositions[] = {
 glm::vec3(0.0f, 0.0f, 0.0f),
 glm::vec3(2.0f, 5.0f, -15.0f),
 glm::vec3(-1.5f, -2.2f, -2.5f),
 glm::vec3(-3.8f, -2.0f, -12.3f),
 glm::vec3(2.4f, -0.4f, -3.5f),
 glm::vec3(-1.7f, 3.0f, -7.5f),
 glm::vec3(1.3f, -2.0f, -2.5f),
 glm::vec3(1.5f, 2.0f, -2.5f),
 glm::vec3(1.5f, 0.2f, -1.5f),
 glm::vec3(-1.3f, 1.0f, -1.5f)
};



using namespace std;

const char* pathProyecto = "../tests/AG09_1/";
#pragma region Cabezeras
void OnChangeFrameBufferSize(GLFWwindow* window, const int32_t width, const int32_t height);
#pragma endregion


#pragma region Metodos


void OnChangeFrameBufferSize(GLFWwindow* window, const int32_t width, const int32_t height) {
	//redimension de pantalla 
	//Cambio de clip scene a view scene
	glViewport(0, 0, width, height);
}

void OnMouse(GLFWwindow* window, double xpos, double ypos) {
	if (_firstMouse) {
		_firstMouse = false;
		_lastX = xpos;
		_lastY = ypos;
	}

	_xoffset = xpos - _lastX;
	_yoffset = ypos - _lastY;
	_lastX = xpos;
	_lastY = ypos;
	camera.handleMouseMovement(_xoffset, _yoffset);
}


void OnScroll(GLFWwindow* window, double xoffset, double yoffset) {
	camera.handleMouseScroll(yoffset);
}

void HandlerInput(GLFWwindow* window, const double deltaTime) {
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.HandleKeyboard(Camera::Movement::Forward, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.HandleKeyboard(Camera::Movement::Backward, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.HandleKeyboard(Camera::Movement::Left, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.HandleKeyboard(Camera::Movement::Right, deltaTime);
	}
	//Window::HandlerInput();
}



int Inicializacion() {
	if (!glfwInit()) {
		cout << "Error initializing GLFW" << endl;
		glfwTerminate();
		return -1;
	}
	window = Window(screen_width, screen_height);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Error initializing GLAD" << endl;
		return -1;
	}
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//cuando la ventana cambie de tamaño
	glfwSetCursorPosCallback(window.GetWindow(), OnMouse);
	glfwSetFramebufferSizeCallback(window.GetWindow(), OnChangeFrameBufferSize);
	glfwSetScrollCallback(window.GetWindow(), OnScroll);
	glfwSetInputMode(window.GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	return 1;
};


void Render(const Shader& shader, const Model& obj) {
	//Renderizamos la pantalla con un color basandonos en el esquema RGBA(transparencia)
	//Si lo quitamos, no borra nunca la pantalla

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 projection = glm::perspective(glm::radians(camera.GetFOV()), 800.0f / 600.0f, 0.1f, 60.0f);

	shader.Use();
	shader.Set("projection", projection);
	shader.Set("view", view);

	glm::mat4 model(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
	shader.Set("model", model);

	obj.Draw(shader);
	glBindVertexArray(0);

}


int main(int argc, char* argv[]) {
	if (!Inicializacion()) {
		return -1;
	}


	string vertexpathStr = utils.GetFinalPath(pathProyecto, "Shaders/vertex.vs");
	const char* vertexpath = vertexpathStr.c_str();

	string fragmentPathString = utils.GetFinalPath(pathProyecto, "Shaders/fragment.fs");
	const char* fragmentPath1 = fragmentPathString.c_str();

	Shader shader = Shader(vertexpath, fragmentPath1);
	int program = shader.GetIdProgram();
	Model object("../assets/obj/Freigther_BI_Export.obj");


	//Bucle inicial donde se realiza toda la accion del motor
	while (!glfwWindowShouldClose(window.GetWindow())) {
		float currentFrame = glfwGetTime();
		float deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		HandlerInput(window.GetWindow(), deltaTime);
		window.HandlerInput();

		Render(shader, object);

		glfwSwapBuffers(window.GetWindow());
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}
#pragma endregion

