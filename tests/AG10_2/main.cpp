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
Camera camera(glm::vec3(-5.0f, 1.0f, 7.0f));
glm::vec3 lightPos(1.2f, 1.0f, -2.0f);
float lastFrame = 0.0f;
bool firstMouse = true;

const uint32_t screen_width = 800, screen_height = 600;
float lastX = (float)screen_width / 2.0f;
float lastY = (float)screen_height / 2.0f;
Window window;


float quad_vertices_suelo[] = {
	// positions		texture cords						// normal coords
	-0.5f, 0.5f, 0.5f,		0.0f, 0.0f,				0.0f, 1.0f, 0.0f,//Top
	0.5f, 0.5f, 0.5f,		1.0f, 0.0f,				0.0f, 1.0f, 0.0f,
	0.5f, 0.5f, -0.5f,		1.0f, 1.0f,				0.0f, 1.0f, 0.0f,
	-0.5f, 0.5f, -0.5f,		0.0f, 1.0f,				0.0f, 1.0f, 0.0f
};

uint32_t quad_indices[] = {
	0, 1, 2, 0, 2, 3 //Front
};

uint32_t numeroElementosVerticesCubo = 192;

float verticesCubo[]{
	-0.5f, -0.5f, 0.5f,		0.0f, 0.0f,				0.0f, 0.0f, 1.0f, //Front
	0.5f, -0.5f, 0.5f,		1.0f, 0.0f,				0.0f, 0.0f, 1.0f,
	0.5f, 0.5f, 0.5f,		1.0f, 1.0f,				0.0f, 0.0f, 1.0f,
	-0.5f, 0.5f, 0.5f,		0.0f, 1.0f,				0.0f, 0.0f, 1.0f,

	0.5f, -0.5f, 0.5f,		0.0f, 0.0f,				1.0f, 0.0f, 0.0f,//Right
	0.5f, -0.5f, -0.5f,		1.0f, 0.0f,				1.0f, 0.0f, 0.0f,
	0.5f, 0.5f, -0.5f,		1.0f, 1.0f,				1.0f, 0.0f, 0.0f,
	0.5f, 0.5f, 0.5f,		0.0f, 1.0f,				1.0f, 0.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,	1.0f, 0.0f,				0.0f, 0.0f, -1.0f,//Back
	-0.5f, 0.5f, -0.5f,		1.0f, 1.0f,				0.0f, 0.0f, -1.0f,
	0.5f, 0.5f, -0.5f,		0.0f, 1.0f,				0.0f, 0.0f, -1.0f,
	0.5f, -0.5f, -0.5f,		 0.0f, 0.0f,			0.0f, 0.0f, -1.0f,

	-0.5f, -0.5f, 0.5f,		1.0f, 0.0f,				-1.0f, 0.0f, 0.0f,//Left
	-0.5f, 0.5f, 0.5f,		1.0f, 1.0f,				-1.0f, 0.0f, 0.0f,
	-0.5f, 0.5f, -0.5f,		0.0f, 1.0f,				-1.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,				 -1.0f, 0.0f, 0.0f,

	-0.5f, -0.5f, 0.5f,		0.0f, 1.0f,				0.0f, -1.0f, 0.0f,//Bottom
	-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,				0.0f, -1.0f, 0.0f,
	0.5f, -0.5f, -0.5f,		1.0f, 0.0f,				0.0f, -1.0f, 0.0f,
	0.5f, -0.5f, 0.5f,		1.0f, 1.0f,				0.0f, -1.0f, 0.0f,

	-0.5f, 0.5f, 0.5f,		0.0f, 0.0f,				0.0f, 1.0f, 0.0f,//Top
	0.5f, 0.5f, 0.5f,		1.0f, 0.0f,				0.0f, 1.0f, 0.0f,
	0.5f, 0.5f, -0.5f,		1.0f, 1.0f,				0.0f, 1.0f, 0.0f,
	-0.5f, 0.5f, -0.5f,		0.0f, 1.0f,				0.0f, 1.0f, 0.0f };



uint32_t numeroIndicesCubo = 36;

uint32_t indicesCubo[]{
	0, 1, 2, 0, 2, 3 //Front
	,4, 5, 6, 4, 6, 7 //Right
	,8, 9, 10, 8, 10, 11 //Back
	,12, 13, 14, 12, 14, 15 //Left
	,16, 17, 18, 16, 18, 19 //Bottom
	,20, 21, 22, 20, 22, 23 //Top
};




using namespace std;

const string pathProyecto = "../tests/AG10_2/";
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

	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);


	//cuando la ventana cambie de tama�o
	glfwSetCursorPosCallback(window.GetWindow(), OnMouse);
	glfwSetFramebufferSizeCallback(window.GetWindow(), OnChangeFrameBufferSize);
	glfwSetScrollCallback(window.GetWindow(), OnScroll);
	glfwSetInputMode(window.GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	return 1;
};

void Draw(mat4 projection, mat4 view, vec3 pos, vec3 scale, mat4 model, Shader shader, uint32_t VAO, uint32_t numberOfElements) {
	shader.Use();
	shader.Set("projection", projection);
	shader.Set("view", view);
	//glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, lightPos);
	model = glm::scale(model, glm::vec3(0.2f));
	model = glm::translate(model, lightPos);

	shader.Set("model", model);
	glBindVertexArray(VAO);

	glDrawElements(GL_TRIANGLES, numberOfElements, GL_UNSIGNED_INT, 0);
}


void Render(uint32_t VAOQuad, uint32_t VAOCubo, const Shader& shaderCube, const Shader& shaderStencil, uint32_t textureAlbedo, uint32_t textureSpecular) {
	//Renderizamos la pantalla con un color basandonos en el esquema RGBA(transparencia)
	//Si lo quitamos, no borra nunca la pantalla
	//Draw(projection, view, lightPos, scale, model, shaderCube, VAOQuad 36);
	//DrawQuad(projection, view, lightPos, scale, model, shaderCube, 36);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	shaderCube.Use();
	shaderCube.Set("objectColor", 1.0f, 0.5f, 0.5f, 0.31f);

	shaderCube.Set("viewPos", camera.GetPosition());
	shaderCube.Set("light.position", lightPos);
	shaderCube.Set("light.ambient", 0.2f, 0.15f, 0.1f);
	shaderCube.Set("light.diffuse", 0.5f, 0.5f, 0.5f);
	shaderCube.Set("light.specular", 1.0f, 1.0f, 1.0f);

	shaderCube.Set("material.ambient", 1.0f, 0.5f, 0.31f);
	shaderCube.Set("material.shininess", 32.2f);
	shaderCube.Set("material.diffuse", 0);
	shaderCube.Set("material.specular", 1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureAlbedo);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureSpecular);


	glm::mat4 projection = glm::perspective(glm::radians(camera.GetFOV()), 800.0f / 600.0f, 0.1f, 100.0f);
	glm::mat4 view = camera.GetViewMatrix();

	shaderCube.Set("projection", projection);
	shaderCube.Set("view", view);

	glm::mat4 model = glm::mat4(1.0f);
	vec3 pos = vec3(0.0f, -6.0f, 0.0f);
	vec3 scale = vec3(10.0f, 10.f, 10.0f);
	model = glm::translate(model, pos); //Derecha-Izq  Arriba Abajo Z	
	model = glm::scale(model, scale);
	shaderCube.Set("model", model);

	glm::mat3 normalMat = glm::inverse(glm::transpose(glm::mat3(model)));
	shaderCube.Set("normalMat", normalMat);

	glStencilMask(0x00); //Quitamos mascara del Stencil por lo que no guarda
	glBindVertexArray(VAOQuad);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	//1 Pasada donde pintamos los cubos normalmente y guardamos los valores de los cubos
	////Pintamos Cubo 1__________________________________________________________________
	glStencilMask(0xFF); //Activamos mascara del Stencil 
	glStencilFunc(GL_ALWAYS, 1, 0xFF); //Almacena los valores escritos con 1

	glm::mat4 model2 = glm::mat4(1.0f);
	pos = vec3(0.0f, 0.2f, 2.0f);
	scale = vec3(1.0f, 1.0f, 1.0f);
	model2 = glm::translate(model2, pos);
	model2 = glm::scale(model2, scale);
	shaderCube.Set("model", model2);

	normalMat = glm::inverse(glm::transpose(glm::mat3(model2)));
	shaderCube.Set("normalMat", normalMat);

	glBindVertexArray(VAOCubo);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	//Pintamos Cubo 2__________________________________________________________________

	model2 = glm::mat4(1.0f);
	pos = vec3(0.0f, 0.2f, 0.0f);
	model2 = glm::translate(model2, pos);
	model2 = glm::scale(model2, scale);
	shaderCube.Set("model", model2);

	normalMat = glm::inverse(glm::transpose(glm::mat3(model2)));
	shaderCube.Set("normalMat", normalMat);

	glBindVertexArray(VAOCubo);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


	//Pintamos Cubo 3__________________________________________________________________

	model2 = glm::mat4(1.0f);
	pos = vec3(0.0f, 0.2f, -2.0f);
	model2 = glm::translate(model2, pos);
	model2 = glm::scale(model2, scale);
	shaderCube.Set("model", model2);

	normalMat = glm::inverse(glm::transpose(glm::mat3(model2)));
	shaderCube.Set("normalMat", normalMat);

	glBindVertexArray(VAOCubo);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	//2 Pasada donde pintamos los cubos un poco mas grandes, para que el stencil pille la diferencia de tama�o y podamos trabajar con esa diferencia

	////Pintamos Cubo 1__________________________________________________________________

	shaderStencil.Use();
	shaderStencil.Set("projection", projection);
	shaderStencil.Set("view", view);
	glStencilMask(0x00); //Activamos mascara del Stencil en modo lectura 
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF); //Almacena los valores escritos con 1
	glDisable(GL_DEPTH_TEST);
	model2 = glm::mat4(1.0f);
	pos = vec3(0.0f, 0.2f, 2.0f);
	scale = vec3(1.2f, 1.2f, 1.2f);
	model2 = glm::translate(model2, pos);
	model2 = glm::scale(model2, scale);
	shaderStencil.Set("model", model2);

	normalMat = glm::inverse(glm::transpose(glm::mat3(model2)));
	shaderStencil.Set("normalMat", normalMat);

	glBindVertexArray(VAOCubo);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	//Pintamos Cubo 2__________________________________________________________________

	model2 = glm::mat4(1.0f);
	pos = vec3(0.0f, 0.2f, 0.0f);
	model2 = glm::translate(model2, pos);
	model2 = glm::scale(model2, scale);
	shaderStencil.Set("model", model2);

	normalMat = glm::inverse(glm::transpose(glm::mat3(model2)));
	shaderStencil.Set("normalMat", normalMat);

	glBindVertexArray(VAOCubo);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


	//Pintamos Cubo 3__________________________________________________________________

	model2 = glm::mat4(1.0f);
	pos = vec3(0.0f, 0.2f, -2.0f);
	model2 = glm::translate(model2, pos);
	model2 = glm::scale(model2, scale);
	shaderStencil.Set("model", model2);

	normalMat = glm::inverse(glm::transpose(glm::mat3(model2)));
	shaderStencil.Set("normalMat", normalMat);

	glBindVertexArray(VAOCubo);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glStencilMask(0xFF);
	glEnable(GL_DEPTH_TEST);

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
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
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

	string vertexStencilpathStr = utils.GetFinalPath(pathProyecto, "Shaders/Stencil.vs");
	const char* vertexStencilpath = vertexStencilpathStr.c_str();

	string fragmentStencilPathString = utils.GetFinalPath(pathProyecto, "Shaders/fragmentStencil.fs");
	const char* fragmentStencilPath1 = fragmentStencilPathString.c_str();

	string pathFinalImagen1String = utils.GetFinalPath(pathProyecto, "Textures/albedo.png");
	const char* pathFinalImagen1 = pathFinalImagen1String.c_str();

	string pathFinalImagen2String = utils.GetFinalPath(pathProyecto, "Textures/specular.png");
	const char* pathFinalImagen2 = pathFinalImagen2String.c_str();

	uint32_t textDiffuse = createTexture(pathFinalImagen1, true);
	uint32_t textureSpecular = createTexture(pathFinalImagen2, true);

	Shader shader = Shader(vertexpath, fragmentPath1);
	Shader shaderStencil = Shader(vertexStencilpath, fragmentStencilPath1);

	long sizeOfIndices, sizeOfVertices;

	sizeOfIndices = numeroIndicesCubo * sizeof(float);
	sizeOfVertices = numeroElementosVerticesCubo * sizeof(float);


	Buffer buffer = Buffer(sizeOfIndices, sizeOfVertices);

	uint32_t numberOfElementsToDraw = buffer.GetElementsToDraw();

	uint32_t VAOCubo = createVertexData(verticesCubo, numeroElementosVerticesCubo, indicesCubo, numeroIndicesCubo);
	uint32_t VAOQuad = createVertexData(quad_vertices_suelo, numeroElementosVerticesCubo / 6, quad_indices, numeroIndicesCubo / 6);

	//uint32_t VAO = buffer.CreateVAO(&VBOFigura, &EBO, indexes, sizeOfIndices, vertex, sizeOfVertices, &shader);

	//Bucle inicial donde se realiza toda la accion del motor
	while (!glfwWindowShouldClose(window.GetWindow())) {
		float currentFrame = glfwGetTime();
		float deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		HandlerInput(window.GetWindow(), deltaTime);
		window.HandlerInput();

		Render(VAOQuad, VAOCubo, shader, shaderStencil, textDiffuse, textureSpecular);


		glfwSwapBuffers(window.GetWindow());
		glfwPollEvents();
	}

	//Si se han linkado bien los shaders, los borramos ya que estan linkados
	glDeleteVertexArrays(1, &VAOQuad);
	glDeleteVertexArrays(1, &VAOCubo);




	glfwTerminate();
	return 0;
}
#pragma endregion

