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
using namespace std;

const string pathProyecto = "../tests/EJ7_3/";

Utils utils;
Camera camera(glm::vec3(-1.0f, 2.0f, 3.0f));
glm::vec3 lightPos(1.2f, 1.0f, -2.0f);
float lastFrame = 0.0f;
bool firstMouse = true;

const uint32_t screen_width = 800, screen_height = 600;
float lastX = (float)screen_width / 2.0f;
float lastY = (float)screen_height / 2.0f;
Window window;

uint32_t numeroElementosVerticesCubo = 192;

float verticesCubo[]{
	-0.5f, -0.5f, 0.5f,	 0.0f, 0.0f,				0.0f, 0.0f, 1.0f, //Front
	0.5f, -0.5f, 0.5f,	1.0f, 0.0f,					0.0f, 0.0f, 1.0f,
	0.5f, 0.5f, 0.5f,	1.0f, 1.0f,				0.0f, 0.0f, 1.0f,
	-0.5f, 0.5f, 0.5f,	0.0f, 1.0f,							0.0f, 0.0f, 1.0f,

	0.5f, -0.5f, 0.5f,			 0.0f, 0.0f,							1.0f, 0.0f, 0.0f,//Right
	0.5f, -0.5f, -0.5f,		1.0f, 0.0f,								1.0f, 0.0f, 0.0f,
	0.5f, 0.5f, -0.5f,		1.0f, 1.0f,							1.0f, 0.0f, 0.0f,
	0.5f, 0.5f, 0.5f,		0.0f, 1.0f,					1.0f, 0.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,	1.0f, 0.0f,						0.0f, 0.0f, -1.0f,//Back
	-0.5f, 0.5f, -0.5f,		1.0f, 1.0f,					0.0f, 0.0f, -1.0f,
	0.5f, 0.5f, -0.5f,			0.0f, 1.0f,					0.0f, 0.0f, -1.0f,
	0.5f, -0.5f, -0.5f,		 0.0f, 0.0f,				0.0f, 0.0f, -1.0f,

	-0.5f, -0.5f, 0.5f,		1.0f, 0.0f,					-1.0f, 0.0f, 0.0f,//Left
	-0.5f, 0.5f, 0.5f,		1.0f, 1.0f,					-1.0f, 0.0f, 0.0f,
	-0.5f, 0.5f, -0.5f,		0.0f, 1.0f,							-1.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,					 -1.0f, 0.0f, 0.0f,

	-0.5f, -0.5f, 0.5f,		0.0f, 1.0f,						0.0f, -1.0f, 0.0f,//Bottom
	-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,					 0.0f, -1.0f, 0.0f,
	0.5f, -0.5f, -0.5f,		1.0f, 0.0f,					0.0f, -1.0f, 0.0f,
	0.5f, -0.5f, 0.5f,		1.0f, 1.0f,					0.0f, -1.0f, 0.0f,

	-0.5f, 0.5f, 0.5f,		0.0f, 0.0f,					0.0f, 1.0f, 0.0f,//Top
	0.5f, 0.5f, 0.5f,		1.0f, 0.0f,					0.0f, 1.0f, 0.0f,
	0.5f, 0.5f, -0.5f,		1.0f, 1.0f,					0.0f, 1.0f, 0.0f,
	-0.5f, 0.5f, -0.5f,		0.0f, 1.0f,						0.0f, 1.0f, 0.0f };



uint32_t numeroIndicesCubo = 36;

uint32_t indicesCubo[]{
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
	if (firstMouse) {
		firstMouse = false;
		lastX = xpos;
		lastY = ypos;
	}

	double xoffset = xpos - lastX;
	double yoffset = ypos - lastY;
	lastX = xpos;
	lastY = ypos;
	camera.handleMouseMovement(xoffset, yoffset);
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
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//cuando la ventana cambie de tama�o
	glfwSetCursorPosCallback(window.GetWindow(), OnMouse);
	glfwSetFramebufferSizeCallback(window.GetWindow(), OnChangeFrameBufferSize);
	glfwSetScrollCallback(window.GetWindow(), OnScroll);
	glfwSetInputMode(window.GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	return 1;
};

void Render(uint32_t VAO, const Shader& shaderCube, const Shader& shaderlight,
	const uint32_t numberOfElements, Camera camera, uint32_t textureAlbedo, uint32_t textureSpecular) {
	//Renderizamos la pantalla con un color basandonos en el esquema RGBA(transparencia)
	//Si lo quitamos, no borra nunca la pantalla

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	glm::mat4 view = camera.GetViewMatrix();


	glm::mat4 projection = glm::perspective(glm::radians(camera.GetFOV()), 800.0f / 600.0f, 0.1f, 100.0f);

	shaderlight.Use();
	shaderlight.Set("projection", projection);
	shaderlight.Set("view", view);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, lightPos);
	model = glm::scale(model, glm::vec3(0.2f));
	shaderlight.Set("model", model);
	glBindVertexArray(VAO);

	glDrawElements(GL_TRIANGLES, numberOfElements, GL_UNSIGNED_INT, 0);

	shaderCube.Use();
	shaderCube.Set("projection", projection);
	shaderCube.Set("view", view);
	shaderCube.Set("model", glm::mat4(1.0f));
	glm::mat3 normalMat = glm::inverse(glm::transpose(glm::mat3(model)));
	shaderCube.Set("normalMat", normalMat);

	shaderCube.Set("viewPos", camera.GetPosition());
	shaderCube.Set("light.position", lightPos);
	shaderCube.Set("light.ambient", 1.0f, 1.0f, 1.0f);
	shaderCube.Set("light.diffuse", 1.0f, 1.0f, 1.0f);
	shaderCube.Set("light.specular", -1.0f, -1.0f, -1.0f);

	shaderCube.Set("material.ambient", 0.2125f, 0.1275f, 0.054f);
	shaderCube.Set("material.diffuse", 0.714f, 0.4284f, 0.18144f);
	shaderCube.Set("material.specular", -0.393548f, -0.271906f, -0.166721f);
	shaderCube.Set("material.shininess", 25.6f);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureAlbedo);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureSpecular);

	glBindVertexArray(VAO);

	glDrawElements(GL_TRIANGLES, numberOfElements, GL_UNSIGNED_INT, 0);



	//CUBE 2 yellow rubber	
	shaderCube.Use();
	shaderCube.Set("projection", projection);
	shaderCube.Set("view", view);
	mat4 model2 = mat4(1.0f);
	vec3 newPos = { 2.0f, 0.0f, 0.0f };
	model2 = translate(model2, newPos);
	shaderCube.Set("model", model2);
	normalMat = glm::inverse(glm::transpose(glm::mat3(model2)));
	shaderCube.Set("normalMat", normalMat);

	shaderCube.Set("viewPos", camera.GetPosition());
	shaderCube.Set("light.position", lightPos);
	shaderCube.Set("light.ambient", 1.0f, 1.0f, 1.0f);
	shaderCube.Set("light.diffuse", 1.0f, 1.0f, 1.0f);
	shaderCube.Set("light.specular", 1.0f, 1.0f, 1.0f);

	shaderCube.Set("material.ambient", 0.5f, 0.5f, 0.5f);
	shaderCube.Set("material.diffuse", 0.5f, 0.4f, 0.57);
	shaderCube.Set("material.specular", 0.7f, 0.7f, 0.04f);
	shaderCube.Set("material.shininess", 97.65625f);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureAlbedo);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureSpecular);

	glBindVertexArray(VAO);

	glDrawElements(GL_TRIANGLES, numberOfElements, GL_UNSIGNED_INT, 0);



	//CUBE 3 red plastic	
	shaderCube.Use();
	shaderCube.Set("projection", projection);
	shaderCube.Set("view", view);
	model2 = mat4(1.0f);
	newPos = { -2.0f, 0.0f, 0.0f };
	model2 = translate(model2, newPos);
	shaderCube.Set("model", model2);
	normalMat = glm::inverse(glm::transpose(glm::mat3(model2)));
	shaderCube.Set("normalMat", normalMat);

	shaderCube.Set("viewPos", camera.GetPosition());
	shaderCube.Set("light.position", lightPos);
	shaderCube.Set("light.ambient", 1.0f, 1.0f, 1.0f);
	shaderCube.Set("light.diffuse", 1.0f, 1.0f, 1.0f);
	shaderCube.Set("light.specular", 1.0f, 1.0f, 1.0f);

	shaderCube.Set("material.ambient", 0.0f, 0.0f, 0.0f);
	shaderCube.Set("material.diffuse", 0.5f, 0.0f, 0.0f);
	shaderCube.Set("material.specular", 0.7f, 0.6f, 0.6f);
	shaderCube.Set("material.shininess", 32.0f);
		glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureAlbedo);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureSpecular);

	glBindVertexArray(VAO);

	glDrawElements(GL_TRIANGLES, numberOfElements, GL_UNSIGNED_INT, 0);

	//CUBE 4 red plastic	
	shaderCube.Use();
	shaderCube.Set("projection", projection);
	shaderCube.Set("view", view);
	model2 = mat4(1.0f);
	newPos = { -4.0f, 0.0f, 0.0f };
	model2 = translate(model2, newPos);
	shaderCube.Set("model", model2);
	normalMat = glm::inverse(glm::transpose(glm::mat3(model2)));
	shaderCube.Set("normalMat", normalMat);

	shaderCube.Set("viewPos", camera.GetPosition());
	shaderCube.Set("light.position", lightPos);
	shaderCube.Set("light.ambient", 1.0f, 1.0f, 1.0f);
	shaderCube.Set("light.diffuse", 1.0f, 1.0f, 1.0f);
	shaderCube.Set("light.specular", 1.0f, 1.0f,1.0f);

	shaderCube.Set("material.ambient", 0.0f, 0.0f, 0.0f);
	shaderCube.Set("material.diffuse", 0.5f, 0.0f, 0.0f);
	shaderCube.Set("material.specular", -0.7f, -0.6f, -0.6f);
	shaderCube.Set("material.shininess", 32.0f);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureAlbedo);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureSpecular);

	glBindVertexArray(VAO);

	glDrawElements(GL_TRIANGLES, numberOfElements, GL_UNSIGNED_INT, 0);


}
uint32_t createTexture(const char* path, bool flip) {
	uint32_t texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(flip);

	int width, height, nChannels;
	unsigned char* data = stbi_load(path, &width, &height, &nChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
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

	uint32_t _numberOfElementsPerLine = 8;
	uint32_t stride = 3;

	//Bindeamos buffer vertices
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//Subida de vertices al buffer
	glBufferData(GL_ARRAY_BUFFER, n_verts * sizeof(float) * _numberOfElementsPerLine, vertices, GL_STATIC_DRAW);

	//Bindeo buffer indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, n_indices * sizeof(float), indices, GL_STATIC_DRAW);

	//vertices del triangulo 6 por que hay 6 elementos hasta el proximo inicio de linea
	uint32_t atributteNumber = 0;

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, _numberOfElementsPerLine * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	//Vertices de textura
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, _numberOfElementsPerLine * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	stride += 2;
	//Vertices normal
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, _numberOfElementsPerLine * sizeof(float), (void*)(5 * sizeof(float)));
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

	string vertexpathLightString = utils.GetFinalPath(pathProyecto, "Shaders/vertexLight.vs");
	const char* vertexpathLight = vertexpathLightString.c_str();

	string fragmentPathLightString = utils.GetFinalPath(pathProyecto, "Shaders/fragmentLight.fs");
	const char* fragmentPathLight = fragmentPathLightString.c_str();

	string pathFinalImagen1String = utils.GetFinalPath(pathProyecto, "Textures/albedo.png");
	const char* pathFinalImagen1 = pathFinalImagen1String.c_str();

	string pathFinalImagen2String = utils.GetFinalPath(pathProyecto, "Textures/specular.png");
	const char* pathFinalImagen2 = pathFinalImagen2String.c_str();

	uint32_t textDiffuse = createTexture(pathFinalImagen1, true);
	uint32_t textureSpecular = createTexture(pathFinalImagen2, true);

	Shader shader = Shader(vertexpath, fragmentPath1);
	Shader shaderlight = Shader(vertexpathLight, fragmentPathLight);
	int program = shader.GetIdProgram();
	uint32_t VBOFigura, EBO;



	long sizeOfIndices, sizeOfVertices;

	sizeOfIndices = numeroIndicesCubo * sizeof(float);
	sizeOfVertices = numeroElementosVerticesCubo * sizeof(float);


	//float verticesQuad = cuadrado.GetVertexs();
	Buffer buffer = Buffer(sizeOfIndices, sizeOfVertices);
	buffer.SetStatusVerticesColor(false);
	buffer.SetStatusVerticesTextura(true);
	buffer.SetStatusVerticesNormal(true);
	uint32_t numberOfElementsToDraw = buffer.GetElementsToDraw();

	uint32_t VAO = createVertexData(verticesCubo, numeroElementosVerticesCubo, indicesCubo, numeroIndicesCubo);

	//uint32_t VAO = buffer.CreateVAO(&VBOFigura, &EBO, indexes, sizeOfIndices, vertex, sizeOfVertices, &shader);

	//Bucle inicial donde se realiza toda la accion del motor
	while (!glfwWindowShouldClose(window.GetWindow())) {
		float currentFrame = glfwGetTime();
		float deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		HandlerInput(window.GetWindow(), deltaTime);
		window.HandlerInput();
		Render(VAO, shader, shaderlight, numberOfElementsToDraw, camera, textDiffuse, textureSpecular);


		glfwSwapBuffers(window.GetWindow());
		glfwPollEvents();
	}

	//Si se han linkado bien los shaders, los borramos ya que estan linkados
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBOFigura);
	glDeleteBuffers(1, &EBO);



	glfwTerminate();
	return 0;
}
#pragma endregion

