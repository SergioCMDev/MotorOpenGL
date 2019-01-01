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
Camera camera(glm::vec3(-1.0f, 1.0f, 3.0f));
Camera camera2(glm::vec3(5.0f, 1.0f, -1.0f));

glm::vec3 lightPos(1.2f, 1.0f, -2.0f);
float lastFrame = 0.0f;
bool firstMouse = true;

const uint32_t screen_width = 800, screen_height = 600;
float lastX = (float)screen_width / 2.0f;
float lastY = (float)screen_height / 2.0f;
Window window;


float verticesCuadrado[] = {
	// positions		// normal coords	texture cords						
	-0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f, 		0.0f, 0.0f,				//Top
	0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,		1.0f, 0.0f,
	0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,		1.0f, 1.0f,
	-0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,		0.0f, 1.0f,
};

uint32_t indicesCuadrado[] = {
	0, 1, 2, 0, 2, 3 //Front
};

float quad_screen_vertices[] = {
	// positions		texture cords						// normal coords
	-1.0f, 1.0f, 0.0f,		0.0f, 0.0f, 1.0f, 		0.0f, 1.0f,
	-1.0f, -1.0f, 0.0f,		0.0f, 0.0f, 1.0f,		0.0f, 0.0f,
	1.0f, -1.0f, 0.0f,		0.0f, 0.0f, 1.0f,		1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f,

};

uint32_t indicesQuadScreeen[] = {
	0, 1, 2, 0, 2, 3 //Front
};

uint32_t numeroElementosVerticesCubo = 192;

float verticesCubo[]{
	-0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,   0.0f, 0.0f,				 //Front
	0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,   1.0f, 0.0f,
	0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
	-0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,   0.0f, 1.0f,

	0.5f, -0.5f, 0.5f,		1.0f, 0.0f, 0.0f,   0.0f, 0.0f,				//Right
	0.5f, -0.5f, -0.5f,		1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
	0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
	0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 0.0f,   0.0f, 1.0f,

	-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, -1.0f,  1.0f, 0.0f,				//Back
	-0.5f, 0.5f, -0.5f,		0.0f, 0.0f, -1.0f,  1.0f, 1.0f,
	0.5f, 0.5f, -0.5f,		0.0f, 0.0f, -1.0f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,		0.0f, 0.0f, -1.0f,  0.0f, 0.0f,

	-0.5f, -0.5f, 0.5f,		-1.0f, 0.0f, 0.0f,  1.0f, 0.0f,				//Left
	-0.5f, 0.5f, 0.5f,		-1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
	-0.5f, 0.5f, -0.5f,		-1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,	-1.0f, 0.0f, 0.0f,  0.0f, 0.0f,

	-0.5f, -0.5f, 0.5f,		0.0f, -1.0f, 0.0f,  0.0f, 1.0f,				//Bottom
	-0.5f, -0.5f, -0.5f,	0.0f, -1.0f, 0.0f,  0.0f, 0.0f,
	0.5f, -0.5f, -0.5f,		0.0f, -1.0f, 0.0f,  1.0f, 0.0f,
	0.5f, -0.5f, 0.5f,		0.0f, -1.0f, 0.0f,  1.0f, 1.0f,

	-0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,  0.0f, 0.0f,				//Top
	 0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
	 0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
	-0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,  0.0f, 1.0f, };



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

const string pathProyecto = "../tests/EJ12_1/";
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

	//cuando la ventana cambie de tamaño
	glfwSetCursorPosCallback(window.GetWindow(), OnMouse);
	glfwSetFramebufferSizeCallback(window.GetWindow(), OnChangeFrameBufferSize);
	glfwSetScrollCallback(window.GetWindow(), OnScroll);
	glfwSetInputMode(window.GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	return 1;
};


pair<uint32_t, uint32_t> createFBO() {
	uint32_t fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	uint32_t textureColor;
	glGenTextures(1, &textureColor);
	glBindTexture(GL_TEXTURE_2D, textureColor);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screen_width, screen_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColor, 0);

	uint32_t rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, screen_width, screen_height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		cout << "Error loading GL_FRAMEBUFFER" << endl;
	}

	return make_pair(fbo, textureColor);
}

void RenderScena(Camera cameraActual, const Shader& shaderCube, const Shader& shaderFBO,
	uint32_t VAOCubo, uint32_t VAOQuad, uint32_t VAOScreenQuad,
	const uint32_t textureAlbedo, const uint32_t textureSpecular,
	const uint32_t fbo, const uint32_t text_fbo,
	const uint32_t fbo2, const uint32_t text_fbo2) {

	glEnable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);


	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	vec4 color(1.0f, 0.0f, 0.1f, 1.0f);

	shaderCube.Use();
	shaderCube.Set("objectColor", 1.0f, 0.5f, 0.5f);

	shaderCube.Set("viewPos", cameraActual.GetPosition());
	shaderCube.Set("light.position", lightPos);
	shaderCube.Set("light.ambient", 0.2f, 0.15f, 0.1f);
	//Valor Normal
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


	glm::mat4 projection = glm::perspective(glm::radians(cameraActual.GetFOV()), 800.0f / 600.0f, 0.1f, 100.0f);
	glm::mat4 view = cameraActual.GetViewMatrix();

	//Pintamos suelo
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


	glBindVertexArray(VAOQuad);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);





	//////Pintamos Cubo 1

	glm::mat4 model2 = glm::mat4(1.0f);
	pos = vec3(0.0f, 0.2f, 2.0f);
	scale = vec3(1.0f, 1.f, 1.0f);
	//model2 = glm::translate(model2, pos);
	//model2 = glm::scale(model2, scale);
	//shaderCube.Set("model", model2);

	//normalMat = glm::inverse(glm::transpose(glm::mat3(model2)));
	//shaderCube.Set("normalMat", normalMat);

	//glBindVertexArray(VAOCubo);

	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	////Pintamos Cubo 2

	model2 = glm::mat4(1.0f);
	pos = vec3(0.0f, 0.2f, 0.0f);
	model2 = glm::translate(model2, pos);
	model2 = glm::scale(model2, scale);
	shaderCube.Set("model", model2);

	normalMat = glm::inverse(glm::transpose(glm::mat3(model2)));
	shaderCube.Set("normalMat", normalMat);
	glBindVertexArray(VAOCubo);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


	////Pintamos Cubo 3

	model2 = glm::mat4(1.0f);
	pos = vec3(0.0f, 0.2f, -2.0f);
	model2 = glm::translate(model2, pos);
	model2 = glm::scale(model2, scale);
	shaderCube.Set("model", model2);

	normalMat = glm::inverse(glm::transpose(glm::mat3(model2)));
	shaderCube.Set("normalMat", normalMat);

	glBindVertexArray(VAOCubo);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);



	if (fbo2 > 0) {
		//Pintamos suelo
		shaderCube.Use();
		shaderCube.Set("projection", projection);
		shaderCube.Set("view", view);
		glm::mat4 model = glm::mat4(1.0f);
		vec3 pos = vec3(3.0f, 0.2f, -2.0f);
		vec3 scale = vec3(2.0f);
		model = glm::translate(model, pos); //Derecha-Izq  Arriba Abajo Z	
		model = glm::scale(model, scale);
		shaderCube.Set("model", model);

		glm::mat3 normalMat = glm::inverse(glm::transpose(glm::mat3(model)));
		shaderCube.Set("material.diffuse", 0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, text_fbo);

		glBindVertexArray(VAOScreenQuad);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);

}



void Render(
	const Shader& shaderCube, const Shader& shaderFBO,
	uint32_t VAOCubo, uint32_t VAOQuad, uint32_t VAOScreenQuad,
	const uint32_t textureAlbedo, const uint32_t textureSpecular,
	const uint32_t fbo, const uint32_t text_fbo) {


	glBindVertexArray(VAOScreenQuad);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	auto fboResCamara1 = createFBO();
	auto fboResCamara2 = createFBO();
	RenderScena(camera2, shaderCube, shaderFBO, VAOCubo, VAOQuad, VAOScreenQuad, textureAlbedo, textureSpecular, fboResCamara2.first, fboResCamara2.second,0,0);
	RenderScena(camera, shaderCube, shaderFBO, VAOCubo, VAOQuad, VAOScreenQuad, textureAlbedo, textureSpecular, fboResCamara1.first, fboResCamara1.second, fboResCamara2.first, fboResCamara2.second);




	//FIN PASADA 2
	//Para mostrar la linea en medio
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glDisable(GL_DEPTH_TEST);

	//Pintamos sobre gris
	//glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	//Pintamos sobre blanco
	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	shaderFBO.Use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fboResCamara1.second);
	shaderFBO.Set("screenTexture", 0);

	glBindVertexArray(VAOScreenQuad);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

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

	//Bindeamos buffer vertices
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//Subida de vertices al buffer
	glBufferData(GL_ARRAY_BUFFER, n_verts * sizeof(float) * _numberOfElementsPerLine, vertices, GL_STATIC_DRAW);

	//Bindeo buffer indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, n_indices * sizeof(float), indices, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, _numberOfElementsPerLine * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	//Vertices normal
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

	string vertexpathFboStr = utils.GetFinalPath(pathProyecto, "Shaders/fbo.vs");
	const char* vertexpathFbo = vertexpathFboStr.c_str();

	string fragmentPathFboString = utils.GetFinalPath(pathProyecto, "Shaders/fbo.fs");
	const char* fragmentPathFbo = fragmentPathFboString.c_str();

	string pathFinalImagen1String = utils.GetFinalPath(pathProyecto, "Textures/albedo.png");
	const char* pathFinalImagen1 = pathFinalImagen1String.c_str();

	string pathFinalImagen2String = utils.GetFinalPath(pathProyecto, "Textures/specular.png");
	const char* pathFinalImagen2 = pathFinalImagen2String.c_str();

	uint32_t textDiffuse = createTexture(pathFinalImagen1, true);
	uint32_t textureSpecular = createTexture(pathFinalImagen2, true);



	Shader shader = Shader(vertexpath, fragmentPath1);

	Shader fboShader = Shader(vertexpathFbo, fragmentPathFbo);

	auto fboRes = createFBO();

	uint32_t VAOCubo = createVertexData(verticesCubo, 24, indicesCubo, 36);
	uint32_t VAOQuad = createVertexData(verticesCuadrado, 4, indicesCuadrado, 6);
	uint32_t VAOScreenQuad = createVertexData(quad_screen_vertices, 4, indicesQuadScreeen, 6);


	//Bucle inicial donde se realiza toda la accion del motor
	while (!glfwWindowShouldClose(window.GetWindow())) {
		float currentFrame = glfwGetTime();
		float deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		HandlerInput(window.GetWindow(), deltaTime);
		window.HandlerInput();

		Render(shader, fboShader, VAOCubo, VAOQuad, VAOScreenQuad, textDiffuse, textureSpecular, fboRes.first, fboRes.second);

		glfwSwapBuffers(window.GetWindow());
		glfwPollEvents();
	}

	//Si se han linkado bien los shaders, los borramos ya que estan linkados
	glDeleteVertexArrays(1, &VAOQuad);
	glDeleteVertexArrays(1, &VAOCubo);
	glDeleteVertexArrays(1, &VAOScreenQuad);




	glfwTerminate();
	return 0;
}
#pragma endregion

