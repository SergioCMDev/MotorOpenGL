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

float verticesQuadFrontal[]{
	-0.5f, -0.5f, 0.5f,		0.0f, 0.0f,				0.0f, 0.0f, 1.0f, //Front
	0.5f, -0.5f, 0.5f,		1.0f, 0.0f,				0.0f, 0.0f, 1.0f,
	0.5f, 0.5f, 0.5f,		1.0f, 1.0f,				0.0f, 0.0f, 1.0f,
	-0.5f, 0.5f, 0.5f,		0.0f, 1.0f,				0.0f, 0.0f, 1.0f,
};



uint32_t numeroIndicesCubo = 36;

uint32_t indicesQuadFrontal[]{
	0, 1, 2, 0, 2, 3 //Front
};




using namespace std;

const string pathProyecto = "../tests/EJ10_2/";
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

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	//cuando la ventana cambie de tamaño
	glfwSetCursorPosCallback(window.GetWindow(), OnMouse);
	glfwSetFramebufferSizeCallback(window.GetWindow(), OnChangeFrameBufferSize);
	glfwSetScrollCallback(window.GetWindow(), OnScroll);
	glfwSetInputMode(window.GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	return 1;
};

void Render(uint32_t VAOQuad, uint32_t VAOCubo, 
	const Shader& shaderQuad, const Shader& blendShader, const Shader& shaderCube,
	uint32_t textGrass, uint32_t textureWood){
	//Renderizamos la pantalla con un color basandonos en el esquema RGBA(transparencia)
	//Si lo quitamos, no borra nunca la pantalla


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//Pintamos Suelo
	shaderQuad.Use();
	shaderQuad.Set("objectColor", 1.0f, 0.5f, 0.5f, 0.31f);

	shaderQuad.Set("viewPos", camera.GetPosition());
	shaderQuad.Set("light.position", lightPos);
	shaderQuad.Set("light.ambient", 0.2f, 0.15f, 0.1f);
	shaderQuad.Set("light.diffuse", 0.5f, 0.5f, 0.5f);
	shaderQuad.Set("light.specular", 1.0f, 1.0f, 1.0f);

	shaderQuad.Set("material.ambient", 1.0f, 0.5f, 0.31f);
	shaderQuad.Set("material.shininess", 32.2f);
	shaderQuad.Set("material.diffuse", 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textGrass);


	glm::mat4 projection = glm::perspective(glm::radians(camera.GetFOV()), 800.0f / 600.0f, 0.1f, 100.0f);
	glm::mat4 view = camera.GetViewMatrix();

	shaderQuad.Set("projection", projection);
	shaderQuad.Set("view", view);

	glm::mat4 modelQuad = glm::mat4(1.0f);
	vec3 pos = vec3(0.0f, -5.5f, 0.0f);
	vec3 scale = vec3(10.0f, 10.f, 10.0f);
	modelQuad = glm::translate(modelQuad, pos); //Derecha-Izq  Arriba Abajo Z	
	modelQuad = glm::scale(modelQuad, scale);
	shaderQuad.Set("model", modelQuad);

	glm::mat3 normalMat = glm::inverse(glm::transpose(glm::mat3(modelQuad)));
	shaderQuad.Set("normalMat", normalMat);

	glBindVertexArray(VAOQuad);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	////Pintamos Cubo 1__________________________________________________________________

	shaderCube.Use();

	shaderCube.Set("projection", projection);
	shaderCube.Set("view", view);
	glm::mat4 modelCube = glm::mat4(1.0f);
	vec3 posCubes = vec3(0.0f, 0.0f, 2.0f);
	vec3 scaleCubes = vec3(1.0f);
	modelCube = glm::translate(modelCube, posCubes);
	modelCube = glm::scale(modelCube, scaleCubes);
	shaderCube.Set("model", modelCube);
	shaderCube.Set("color", vec3(1.0f, 0.0f, 0.0f));
	shaderCube.Set("transparencia", 0.2f);

	normalMat = glm::inverse(glm::transpose(glm::mat3(modelCube)));
	shaderCube.Set("normalMat", normalMat);

	glBindVertexArray(VAOCubo);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	////Pintamos Cubo 2__________________________________________________________________


	modelCube = glm::mat4(1.0f);
	posCubes = vec3(0.0f, 0.0f, -2.0f);
	scaleCubes = vec3(1.0f);
	modelCube = glm::translate(modelCube, posCubes);
	modelCube = glm::scale(modelCube, scaleCubes);
	shaderCube.Set("model", modelCube);
	shaderCube.Set("color", vec3(0.0f, 1.0f, 0.0f));
	shaderCube.Set("transparencia", 0.7f);

	normalMat = glm::inverse(glm::transpose(glm::mat3(modelCube)));
	shaderCube.Set("normalMat", normalMat);

	glBindVertexArray(VAOCubo);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


	//////Pintamos Cubo 3__________________________________________________________________

	modelCube = glm::mat4(1.0f);
	posCubes = vec3(0.0f, 0.0f, 0.0f);
	scaleCubes = vec3(1.0f);
	modelCube = glm::translate(modelCube, posCubes);
	modelCube = glm::scale(modelCube, scaleCubes);
	shaderCube.Set("model", modelCube);
	shaderCube.Set("color", vec3(0.0f, 0.0f, 1.0f));
	shaderCube.Set("transparencia", 0.5f);

	normalMat = glm::inverse(glm::transpose(glm::mat3(modelCube)));
	shaderCube.Set("normalMat", normalMat);

	glBindVertexArray(VAOCubo);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


	//////Pintamos Cubo 4__________________________________________________________________

	shaderCube.Set("projection", projection);
	shaderCube.Set("view", view);
	modelCube = glm::mat4(1.0f);
	posCubes = vec3(0.0f, 0.0f, 0.0f);
	scaleCubes = vec3(1.0f);
	modelCube = glm::translate(modelCube, posCubes);
	modelCube = glm::scale(modelCube, scaleCubes);
	shaderCube.Set("model", modelCube);
	shaderCube.Set("color", vec3(0.0f, 0.0f, 1.0f));
	shaderCube.Set("transparencia", 0.3f);

	normalMat = glm::inverse(glm::transpose(glm::mat3(modelCube)));
	shaderCube.Set("normalMat", normalMat);

	glBindVertexArray(VAOCubo);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


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
	stbi_image_free(data);
	return texture;
}

uint32_t createTextureAlpha(const char* path, bool flip) {
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
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
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
	string quadVSPathString = utils.GetFinalPath(pathProyecto, "Shaders/quad.vs");
	const char* quadVSPath = quadVSPathString.c_str();

	string quadFSPathString = utils.GetFinalPath(pathProyecto, "Shaders/quad.fs");
	const char* quadFSPath = quadFSPathString.c_str();

	string cubeVSPathStr = utils.GetFinalPath(pathProyecto, "Shaders/cube.vs");
	const char* cubeVSPath = cubeVSPathStr.c_str();

	string cubeFSPathStr = utils.GetFinalPath(pathProyecto, "Shaders/cube.fs");
	const char* cubeFSPath = cubeFSPathStr.c_str();

	string vertexBlendpathStr = utils.GetFinalPath(pathProyecto, "Shaders/blend.vs");
	const char* BlendVSPath = vertexBlendpathStr.c_str();

	string fragmentBlendPathString = utils.GetFinalPath(pathProyecto, "Shaders/fragmentBlend.fs");
	const char* BlendFSPath = fragmentBlendPathString.c_str();

	string pathFinalImagen1String = utils.GetFinalPath(pathProyecto, "Textures/grass.png");
	const char* textureGrassPath = pathFinalImagen1String.c_str();

	string pathFinalImagen2String = utils.GetFinalPath(pathProyecto, "Textures/tree.png");
	const char* textureWoodPath = pathFinalImagen2String.c_str();

	uint32_t textGrass = createTexture(textureGrassPath, true);
	uint32_t textureWood = createTexture(textureWoodPath, true);

	Shader shaderQuad = Shader(quadVSPath, quadFSPath);
	Shader shaderBlend = Shader(BlendVSPath, BlendFSPath);
	Shader shaderCubos = Shader(cubeVSPath, cubeFSPath);

	long sizeOfIndices, sizeOfVertices;


	uint32_t VAOCubo = createVertexData(verticesQuadFrontal, numeroElementosVerticesCubo, indicesQuadFrontal, numeroIndicesCubo);
	uint32_t VAOQuad = createVertexData(quad_vertices_suelo, numeroElementosVerticesCubo / 6, quad_indices, numeroIndicesCubo / 6);


	//Bucle inicial donde se realiza toda la accion del motor
	while (!glfwWindowShouldClose(window.GetWindow())) {
		float currentFrame = glfwGetTime();
		float deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		HandlerInput(window.GetWindow(), deltaTime);
		window.HandlerInput();

		Render(VAOQuad, VAOCubo, shaderQuad, shaderBlend, shaderCubos, textGrass, textureWood);


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

