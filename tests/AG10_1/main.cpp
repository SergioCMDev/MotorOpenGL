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
#include "string.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Utils utils;
Camera camera(glm::vec3(-1.0f, 2.0f, 3.0f));
glm::vec3 lightPos(1.2f, 1.0f, -2.0f);
float lastFrame = 0.0f;
bool firstMouse = true;

const float screen_width = 800.0f, screen_height = 600.0f;
float lastX = (float)screen_width / 2.0f;
float lastY = (float)screen_height / 2.0f;
Window window;
//UtilsHandlers handlers;

bool _firstMouse = false;
double _lastX, _lastY, _xoffset, _yoffset;
uint32_t _elementsVertexs = 192;

float vertexCube[] = {
	-0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f,		//Front
	0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	1.0f, 0.0f,
	0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	1.0f, 1.0f,
	-0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	0.0f, 1.0f,

	0.5f, -0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	0.0f, 0.0f,		//Right
	0.5f, -0.5f, -0.5f,		1.0f, 0.0f, 0.0f,	1.0f, 0.0f,
	0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 0.0f,	1.0f, 1.0f,
	0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	0.0f, 1.0f,

	-0.5f, -0.5f, -0.5f	,	0.0f, 0.0f, -1.0f,	1.0f, 0.0f,		//Back
	-0.5f, 0.5f, -0.5f,		0.0f, 0.0f, -1.0f,	1.0f, 1.0f,
	0.5f, 0.5f, -0.5f,		0.0f, 0.0f, -1.0f,	0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,		0.0f, 0.0f, -1.0f,	0.0f, 0.0f,

	-0.5f, -0.5f, 0.5f,		-1.0f, 0.0f, 0.0f,	1.0f, 0.0f,		//Left	
	-0.5f, 0.5f, 0.5f,		-1.0f, 0.0f, 0.0f,	1.0f, 1.0f,
	-0.5f, 0.5f, -0.5f,		-1.0f, 0.0f, 0.0f,	0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,	-1.0f, 0.0f, 0.0f,	0.0f, 0.0f,

	-0.5f, -0.5f, 0.5f,		0.0f, -1.0f, 0.0f,	0.0f, 1.0f,		//Bottom
	-0.5f, -0.5f, -0.5f,	0.0f, -1.0f, 0.0f,	0.0f, 0.0f,
	0.5f, -0.5f, -0.5f,		0.0f, -1.0f, 0.0f,	1.0f, 0.0f,
	0.5f, -0.5f, 0.5f,		0.0f, -1.0f, 0.0f,	1.0f, 1.0f,

	-0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	0.0f, 0.0f,		//Top
	0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f,
	0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	1.0f, 1.0f,
	-0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f ,	0.0f, 1.0f 
};
uint32_t _elementsVertexsQuad = 32;

float vertexQuad[] = {
	-0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f,		//Front
	0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	1.0f, 0.0f,
	0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	1.0f, 1.0f,
	-0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	0.0f, 1.0f };

uint32_t elementsIndexes = 36;

uint32_t indexesCube[] = {
	0, 1, 2, 0, 2, 3 //Front
	,4, 5, 6, 4, 6, 7 //Right
	,8, 9, 10, 8, 10, 11 //Back
	,12, 13, 14, 12, 14, 15 //Left
	,16, 17, 18, 16, 18, 19 //Bottom
	,20, 21, 22, 20, 22, 23 //Top
};
uint32_t indexesQuad[] = {
	0, 1, 2, 0, 2, 3 //Front
};

using namespace std;

const string pathProyecto = "../tests/AG10_1/";
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
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);

	//cuando la ventana cambie de tamaño
	glfwSetCursorPosCallback(window.GetWindow(), OnMouse);
	glfwSetFramebufferSizeCallback(window.GetWindow(), OnChangeFrameBufferSize);
	glfwSetScrollCallback(window.GetWindow(), OnScroll);
	//glfwSetInputMode(window.GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	return 1;
};

void Render(const Shader& shaderlight, const uint32_t VAOCube, const uint32_t VAOQuad, uint32_t texture1, uint32_t texture2) {
//void Render(const Shader& shaderlight, const uint32_t VAOQuad, uint32_t texture1, uint32_t texture2) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	vec4 color(1.0f, 0.0f, 0.0f, 1.0f);
	//Definimos cubo de luz
	uint32_t numberOfElements = 6;
	shaderlight.Use();
	shaderlight.Set("objectColor", 1.0f, 0.5f, 0.31f);

	shaderlight.Set("viewPos", camera.GetPosition());
	shaderlight.Set("light.position", lightPos);
	shaderlight.Set("light.ambient", 0.2f, 0.2f, 0.2f);
	shaderlight.Set("light.diffuse", 0.5f, 0.5f, 0.5f);
	shaderlight.Set("light.specular", 1.0f, 1.0f, 1.0f);

	shaderlight.Set("material.ambient", 1.0f, 0.5f, 0.31f);
	shaderlight.Set("material.diffuse", 1.0f, 0.5f, 0.31f);
	shaderlight.Set("material.shininess", 32.0f);
	shaderlight.Set("material.diffuse", 0);
	shaderlight.Set("material.specular", 1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(camera.GetFOV()), 800.f / 600.f, 0.1f, 100.0f);
	glm::mat4 view = camera.GetViewMatrix();

	shaderlight.Set("projection", projection);
	shaderlight.Set("view", view);
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(3.0f, 1.0f, 3.0f));
	shaderlight.Set("model", model);

	glm::mat3 normalMat = glm::inverse(glm::transpose(glm::mat3(model)));
	shaderlight.Set("normalMat", normalMat);

	glBindVertexArray(VAOQuad);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	numberOfElements = 36;

	////Definicion y pintado de cubo1
	model = glm::mat4(1.0f);
	vec3 posCubo = glm::vec3(0.0f, 0.2f, 1.0f);
	model = glm::translate(model, posCubo);
	model = glm::scale(model, glm::vec3(0.4f));
	shaderlight.Set("model", model);

	normalMat = glm::inverse(glm::transpose(glm::mat3(model)));
	shaderlight.Set("normalMat", normalMat);

	glBindVertexArray(VAOCube);
	glDrawElements(GL_TRIANGLES, numberOfElements, GL_UNSIGNED_INT, 0);

	//Definicion y pintado de cubo2
	model = glm::mat4(1.0f);
	posCubo = glm::vec3(0.0f, 0.2f, 0.0f);
	model = glm::translate(model, posCubo);
	model = glm::scale(model, glm::vec3(0.4f));
	shaderlight.Set("model", model);
	normalMat = glm::inverse(glm::transpose(glm::mat3(model)));

	shaderlight.Set("normalMat", normalMat);
	glBindVertexArray(VAOCube);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	//Definicion y pintado de cubo3

	model = glm::mat4(1.0f);
	posCubo = glm::vec3(-1.0f, 0.2f, 0.0f);
	model = glm::translate(model, posCubo);
	model = glm::scale(model, glm::vec3(0.4f));
	shaderlight.Set("model", model);

	normalMat = glm::inverse(glm::transpose(glm::mat3(model)));
	shaderlight.Set("normalMat", normalMat);

	glBindVertexArray(VAOCube);
	glDrawElements(GL_TRIANGLES, numberOfElements, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}


uint32_t createTexture(const char* path, bool flip) {
	uint32_t texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(flip);	int width, height, nChannels;
	unsigned char* data = stbi_load(path, &width, &height, &nChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		cout << "Fallo al subir la texture" << endl;
	}
	stbi_image_free(data);
	return texture;
}

uint32_t createVertexData(const float* vertices, const uint32_t n_verts, const uint32_t* indices, const uint32_t n_indices) {
	unsigned int VAO, VBO, EBO;

	glGenVertexArrays(1, &VAO);
	//Generamos 2 buffer, elementos y objetos
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	//Bindeamos el VAO
	glBindVertexArray(VAO);


	//Bindeamos buffer vertices
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//Subida de vertices al buffer
	uint32_t _numberOfElementsPerLine = 8;
	glBufferData(GL_ARRAY_BUFFER, n_verts * sizeof(float)  * _numberOfElementsPerLine, vertices, GL_STATIC_DRAW);

	//Bindeo buffer indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, n_indices * sizeof(uint32_t), indices, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, _numberOfElementsPerLine * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Vertices de normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, _numberOfElementsPerLine * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	//Vertices de textura
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, _numberOfElementsPerLine * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	//desbindeamos buffer objetos
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Desbindeo
	glBindVertexArray(0);

	//desbindeamos buffer elements
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return VAO;
}

int main(int argc, char* argv[]) {
	if (!Inicializacion()) {
		return -1;
	}
	string vertexpathStr = utils.GetFinalPath(pathProyecto, "Shaders/vertex.vs");
	const char* vertexpath = vertexpathStr.c_str();

	string fragmentPathString = utils.GetFinalPath(pathProyecto, "Shaders/fragment.fs");
	const char* fragmentPath1 = fragmentPathString.c_str();

	string pathFinalImagen1String = utils.GetFinalPath(pathProyecto, "Textures/albedo.png");
	const char* pathFinalImagen1 = pathFinalImagen1String.c_str();

	string pathFinalImagen2String = utils.GetFinalPath(pathProyecto, "Textures/specular.png");
	const char* pathFinalImagen2 = pathFinalImagen2String.c_str();

	uint32_t texture1 = createTexture(pathFinalImagen1, true);
	uint32_t texture2 = createTexture(pathFinalImagen2, true);

	Shader shader = Shader(vertexpath, fragmentPath1);

	uint32_t VAOCube = createVertexData(vertexCube, _elementsVertexs, indexesCube, elementsIndexes);
	uint32_t VAOQuad = createVertexData(vertexQuad, _elementsVertexsQuad, indexesQuad, elementsIndexes/6);

	//Bucle inicial donde se realiza toda la accion del motor
	while (!glfwWindowShouldClose(window.GetWindow())) {
		float currentFrame = glfwGetTime();
		float deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		HandlerInput(window.GetWindow(), deltaTime);
		window.HandlerInput();

		Render(shader, VAOCube, VAOQuad, texture1, texture2);

		glfwSwapBuffers(window.GetWindow());
		glfwPollEvents();
	}

	//Si se han linkado bien los shaders, los borramos ya que estan linkados
	glDeleteVertexArrays(1, &VAOCube);
	glDeleteVertexArrays(1, &VAOQuad);


	glfwTerminate();
	return 0;
}
#pragma endregion

