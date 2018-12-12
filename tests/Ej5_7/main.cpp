#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<iostream>
#include<cstdint>
#include<stdio.h>

#include "Shader.h"
#include "Utils.h"
#include "Window.h"
#include "Buffer.h"
#include "Texture.h"
#include "Camera.h"

Utils utils;
using namespace std;
Window window;

glm::vec3 cubePositions[] = {
 glm::vec3(-4.0f, 4.0f, 0.0f),
 glm::vec3(-2.0f, 2.0f, 0.0f),
 glm::vec3(0.0f, 0.0f, 0.0f),
  glm::vec3(4.0f, 4.0f, 0.0f),
 glm::vec3(2.0f, 2.0f, 0.0f),
 glm::vec3(-4.0f, -4.0f, 0.0f),
 glm::vec3(-2.0f, -2.0f, 0.0f),
  glm::vec3(4.0f, -4.0f, 0.0f),
 glm::vec3(2.0f, -2.0f, 0.0f),
};
Camera camera(glm::vec3(0.0f, -5.0f, 0.0f));
float lastFrame = 0.0f;
bool firstMouse = true;

const int widht = 800, height = 600;
const char* pathProyecto = "../tests/EJ5_6/";
bool _firstMouse = false;
double _lastX, _lastY, _xoffset, _yoffset;


uint32_t _elementsQuad = 20;
float vertexCuad[]{
	// Position					// UVs
	-0.5f, -0.5f, 0.5f,		 0.0f, 0.0f,	//Front	
		0.5f, -0.5f, 0.5f,		 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f,		1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f,		0.0f, 1.0f,
};
uint32_t _elementsVertexs = 120;

uint32_t elementsIndexesQuad = 6;

uint32_t indexesQuad[]{
	0, 1, 2, 0, 2, 3 //Front
};


float vertex[]{
	// Position					// UVs
	-0.5f, -0.5f, 0.5f,		 0.0f, 0.0f,	//Front	
		0.5f, -0.5f, 0.5f,		 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f,		1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f,		0.0f, 1.0f,

		0.5f, -0.5f, 0.5f,		0.0f, 0.0f, //Right
		0.5f, -0.5f, -0.5f,		 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f,		1.0f, 1.0f,
		0.5f, 0.5f, 0.5f,		 0.0f, 1.0f,

		-0.5f, -0.5f, -0.5f,		1.0f, 0.0f, //Back
		-0.5f, 0.5f, -0.5f,		 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f,		0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,		 0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f,		 1.0f, 0.0f, //Left
		-0.5f, 0.5f, 0.5f,		1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f,			0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,		0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f,			0.0f, 1.0f, //Bottom
		-0.5f, -0.5f, -0.5f,		 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,			1.0f, 0.0f,
		0.5f, -0.5f, 0.5f,		1.0f, 1.0f,

		-0.5f, 0.5f, 0.5f,		0.0f, 0.0f, //Top
		0.5f, 0.5f, 0.5f,		1.0f, 0.0f,
		0.5f, 0.5f, -0.5f,		1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f,		 0.0f, 1.0f
};

uint32_t elementsIndexes = 36;

uint32_t indexes[]{
	0, 1, 2, 0, 2, 3 //Front
	,4, 5, 6, 4, 6, 7 //Right
	,8, 9, 10, 8, 10, 11 //Back
	,12, 13, 14, 12, 14, 15 //Left
	,16, 17, 18, 16, 18, 19 //Bottom
	,20, 21, 22, 20, 22, 23 //Top
};


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
	window = Window(widht, height);

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


void RenderCubo(uint32_t VAO, const Shader& shader, const uint32_t numberOfElements, uint32_t texture1,
	uint32_t texture2, uint32_t numeroRepeticionesElementos, glm::vec3 *cubePositions) {
	//Renderizamos la pantalla con un color basandonos en el esquema RGBA(transparencia)
	//Si lo quitamos, no borra nunca la pantalla
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shader.Use();
	glActiveTexture(GL_TEXTURE0);	glBindTexture(GL_TEXTURE_2D, texture1);	glActiveTexture(GL_TEXTURE1);	glBindTexture(GL_TEXTURE_2D, texture2);
	//Bindeamos VAO
	glBindVertexArray(VAO);
	glm::mat4 view = glm::mat4(1.0f);
	//alejamos el mundo 3D
	glm::mat4 projection = glm::mat4(1.0f);
	//projection = glm::perspective(camera.GetFOV(), 800.0f / 600.0f, 0.1f, 200.0f);
	projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 200.0f);
	
	//Cube 1
	glm::mat4 model = glm::mat4(1.0f);

	model = glm::translate(model, vec3{ -4.0f, -6.7f, -11.0f }); //Derecha-Izq  Arriba Abajo Z
	model = glm::scale(model, vec3{ 0.5f , 0.5f, 0.5 });

	shader.Set("model", model);
	shader.Set("texture1", 0);
	shader.Set("texture2", 1);

	glDrawElements(GL_TRIANGLES, numberOfElements, GL_UNSIGNED_INT, 0);

	//Cube 2
	glm::mat4 model2 = glm::mat4(1.0f);

	model2 = glm::translate(model2, vec3{ 4.0f, -5.7f, -16.0f });
	model2 = glm::scale(model2, vec3{ 1.5f , 1.5f, 1.5f });

	shader.Set("model", model2);
	shader.Set("texture1", 0);
	shader.Set("texture2", 1);

	glDrawElements(GL_TRIANGLES, numberOfElements, GL_UNSIGNED_INT, 0);

	//Cube 3
	glm::mat4 model3 = glm::mat4(1.0f);

	model3 = glm::translate(model3, vec3{ -3.0f, -6.7f, -15.0f });
	model3 = glm::scale(model3, vec3{ 0.25f , 0.25f, 0.25f });

	shader.Set("model", model3);
	shader.Set("texture1", 0);
	shader.Set("texture2", 1);

	glDrawElements(GL_TRIANGLES, numberOfElements, GL_UNSIGNED_INT, 0);
	shader.Set("view", camera.GetViewMatrix());
	shader.Set("projection", projection);	
}


void RenderSuelo(uint32_t VAO, const Shader& shader, const uint32_t numberOfElements, uint32_t texture1, uint32_t texture2) {
	//Renderizamos la pantalla con un color basandonos en el esquema RGBA(transparencia)
	//Si lo quitamos, no borra nunca la pantalla
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shader.Use();
	glActiveTexture(GL_TEXTURE0);	glBindTexture(GL_TEXTURE_2D, texture1);	glActiveTexture(GL_TEXTURE1);	glBindTexture(GL_TEXTURE_2D, texture2);
	//Bindeamos VAO
	glBindVertexArray(VAO);

	//alejamos el mundo 3D
	//view = glm::translate(view, glm::vec3(0.0f, -2.0f, -15.0f));
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -2.0f, -15.0f));
	//view = glm::rotate(view,  -glm::radians(45.0f),glm::vec3(25.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(25.0f, 0.0f, 0.0f));

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);


	//model = glm::translate(model, vec3{ 0.0f, 0.0f, -5.0f });
	model = glm::scale(model, vec3{ 10.0f , 10.0f, 10.0f });

	shader.Set("model", model);
	shader.Set("view", camera.GetViewMatrix());
	shader.Set("projection", projection);

	shader.Set("texture1", 0);
	shader.Set("texture2", 1);

	glDrawElements(GL_TRIANGLES, numberOfElements, GL_UNSIGNED_INT, 0);
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

	Shader shaderCubos = Shader(vertexpath, fragmentPath1);
	Shader shaderSuelo = Shader(vertexpath, fragmentPath1);

	int program = shaderCubos.GetIdProgram();
	uint32_t VBOFigura, EBO, VBOFiguraSuelo, EBOSuelo;



	long sizeOfIndices, sizeOfVertices, sizeOfIndicesQuad, sizeOfVerticesQuad;

	sizeOfIndices = elementsIndexes * sizeof(float);
	sizeOfVertices = _elementsVertexs * sizeof(float);

	sizeOfIndicesQuad = sizeOfIndices / 6;
	sizeOfVerticesQuad = sizeOfVertices / 6;


	//float verticesQuad = cuadrado.GetVertexs();
	Buffer buffer = Buffer(sizeOfIndices, sizeOfVertices);
	Buffer bufferSuelo = Buffer(sizeOfIndicesQuad, sizeOfVerticesQuad);

	buffer.SetStatusVerticesColor(false);
	buffer.SetStatusVerticesTextura(true);
	buffer.SetSizeVerticesNormal(false);
	buffer.SetSizeVerticesTextura(2);

	bufferSuelo.SetStatusVerticesColor(false);
	bufferSuelo.SetStatusVerticesTextura(true);
	bufferSuelo.SetSizeVerticesNormal(false);
	bufferSuelo.SetSizeVerticesTextura(2);

	uint32_t numberOfElementsToDrawForGeometryCubos = buffer.GetElementsToDraw();
	uint32_t numberOfElementsToDrawForGeometrySuelo = bufferSuelo.GetElementsToDraw();

	uint32_t VAOCubos = buffer.CreateVAO(&VBOFigura, &EBO, indexes, sizeOfIndices, vertex, sizeOfVertices, &shaderCubos);

	uint32_t VAOSuelo = buffer.CreateVAO(&VBOFiguraSuelo, &EBOSuelo, indexesQuad, sizeOfIndicesQuad, vertexCuad, sizeOfVerticesQuad, &shaderSuelo);

	Texture image1 = Texture(pathFinalImagen1, 1024, 1024, 1, 0, true);
	image1.LoadTexture();
	Texture image2 = Texture(pathFinalImagen2, 1024, 1024, 1, 0, true);
	image2.LoadTexture();

	uint32_t numberOfElements = (sizeof(cubePositions) / (sizeof(float) * 3));
	//Bucle inicial donde se realiza toda la accion del motor
	while (!glfwWindowShouldClose(window.GetWindow())) {
		float currentFrame = glfwGetTime();
		float deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		HandlerInput(window.GetWindow(), deltaTime);
		window.HandlerInput();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		RenderSuelo(VAOSuelo, shaderSuelo, numberOfElementsToDrawForGeometrySuelo, image1.GetTexture(), image2.GetTexture());
		RenderCubo(VAOCubos, shaderCubos, numberOfElementsToDrawForGeometryCubos, image1.GetTexture(), image2.GetTexture(), numberOfElements, cubePositions);

		glfwSwapBuffers(window.GetWindow());
		glfwPollEvents();
	}

	//Si se han linkado bien los shaders, los borramos ya que estan linkados
	glDeleteVertexArrays(1, &VAOCubos);
	glDeleteBuffers(1, &VBOFigura);
	glDeleteBuffers(1, &EBO);
	image1.ReleaseTexture();
	image2.ReleaseTexture();

	glfwTerminate();
	return 0;
}
#pragma endregion

