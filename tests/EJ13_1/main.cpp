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
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

glm::vec3 lightPos(-2.0f, 4.0f, -1.0f);
float lastFrame = 0.0f;
bool firstMouse = true;

const uint32_t screen_width = 1280, screen_height = 720;
const uint32_t shadow_width = 1024, shadow_height = 1024;
const float shadow_near = 1.0f;
const float shadow_far = 7.5f;
float lastX = (float)screen_width / 2.0f;
float lastY = (float)screen_height / 2.0f;
Window window;

float quad_vertices_suelo[] = {
	// positions				// normal coords	 texture cords						
	 25.0f, -0.5f,  25.0f,		0.0f, 1.0f, 0.0f, 		25.0f,  0.0f,				//Top
	-25.0f, -0.5f,  25.0f,		0.0f, 1.0f, 0.0f,		 0.0f,  0.0f,
	-25.0f, -0.5f, -25.0f,		0.0f, 1.0f, 0.0f,		 0.0f, 25.0f,

	25.0f, -0.5f,   25.0f,		0.0f, 1.0f, 0.0f,		25.0f,  0.0f,
   -25.0f, -0.5f,  -25.0f,		0.0f, 1.0f, 0.0f, 		 0.0f, 25.0f,				//Top
	25.0f, -0.5f,  -25.0f,		0.0f, 1.0f, 0.0f,		25.0f, 25.0f

};
uint32_t quad_indices[] = {
	0, 2, 1,	3, 5, 4 //Front
};


float quad_screen_vertices[] = {
	// positions		// normal coords			texture cords	
	-1.0f, 1.0f, 0.0f,		0.0f, 0.0f, 1.0f, 		0.0f, 1.0f,
	-1.0f, -1.0f, 0.0f,		0.0f, 0.0f, 1.0f,		0.0f, 0.0f,
	1.0f, -1.0f, 0.0f,		0.0f, 0.0f, 1.0f,		1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f,

};
uint32_t indicesQuadScreeen[] = {
	0, 1, 2,	0, 2, 3 //Front
};

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

uint32_t indicesCubo[]{
	0, 1, 2, 0, 2, 3 //Front
	,4, 5, 6, 4, 6, 7 //Right
	,8, 9, 10, 8, 10, 11 //Back
	,12, 13, 14, 12, 14, 15 //Left
	,16, 17, 18, 16, 18, 19 //Bottom
	,20, 21, 22, 20, 22, 23 //Top
};




using namespace std;

const string pathProyecto = "../tests/EJ13_1/";
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
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float _xoffset = xpos - lastX;
	float _yoffset = ypos - lastY;
	lastX = xpos;
	lastY = ypos;
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

	uint32_t depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadow_width, shadow_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f,1.0f ,1.0f ,1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		cout << "Error loading GL_FRAMEBUFFER" << endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return make_pair(fbo, depthMap);
}

void RenderScene(const Shader &shader, const Shader &shaderLight,
	const uint32_t cubeVAO, const uint32_t quadVAO, 
	const uint32_t text1, const uint32_t text2) {


	//Pintamos Suelo
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, text1);
	shader.Set("diffuseTexture", 0);

	mat4 model(1.0f);
	model = translate(model, vec3(10.0f, 0.0f, 0.0f));
	shader.Set("model", model);
	glBindVertexArray(quadVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	//Pintamos Cubos
	//Cubo 1
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, text2);
	shader.Set("diffuseTexture", 0);
	model = mat4(1.0f);
	model = translate(model, vec3(0.0f, 1.5f, 0.0f));
	model = scale(model, vec3(0.5f));
	shader.Set("model", model);
	glBindVertexArray(cubeVAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	//Cubo 2
	model = mat4(1.0f);
	model = translate(model, vec3(0.5f, -0.25f, 0.5f));
	model = scale(model, vec3(0.5f));
	shader.Set("model", model);
	glBindVertexArray(cubeVAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	//Cubo 3
	model = mat4(1.0f);
	model = translate(model, vec3(-1.0f, 0.0f, 2.0f));
	model = rotate(model, radians(60.0f), normalize(vec3(1.0f, 0.0f, 1.0f)));
	model = scale(model, vec3(0.25f));
	shader.Set("model", model);
	glBindVertexArray(cubeVAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);

	//Cubo 4
	shaderLight.Use();
	vec3 lightPos = vec3(-1.0f, 5.0f, 2.0f);
	model = mat4(1.0f);
	model = translate(model, lightPos);
	model = scale(model, vec3(0.25f));
	shaderLight.Set("model", model);
	glBindVertexArray(cubeVAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void Render(
	const Shader& shader, const Shader& depthShader, const Shader& debugShader, const Shader& lightShader,
	uint32_t cubeVAO, uint32_t quadVAO, uint32_t screenQuadVAO,
	const uint32_t tex1, const uint32_t tex2,
	const uint32_t fbo, const uint32_t text_fbo) {

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	//Primera pasada
	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, shadow_near, shadow_far);
	mat4 lightView = lookAt(lightPos, vec3(0.0f), vec3(0.0f, 1.0f, 0.0f));
	mat4 lightSpaceMatrix = lightProjection * lightView;

	depthShader.Use();
	depthShader.Set("lightSpaceMatrix", lightSpaceMatrix);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glClear(GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, shadow_width, shadow_height); //Cambiamos tamaño de pantalla a pantalla de sombra
	RenderScene(depthShader, lightShader, cubeVAO, quadVAO, 0, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, screen_width, screen_height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader.Use();
	lightProjection = glm::perspective(radians(camera.GetFOV()), (float) screen_width/ screen_height, 0.1f, 100.0f);
	mat4 view = camera.GetViewMatrix();
	shader.Set("projection", lightProjection);
	shader.Set("view", view);
	shader.Set("viewPos", camera.GetPosition());
	shader.Set("lightPos", lightPos);
	shader.Set("lightSpaceMatrix", lightSpaceMatrix);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, text_fbo);
	shader.Set("depthMap", 1);
	RenderScene(shader, lightShader, cubeVAO, quadVAO, tex1, tex2);


	//debugShader.Use();
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, text_fbo);
	//debugShader.Set("depthMap", 0);

	//glBindVertexArray(screenQuadVAO);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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
	string vertexpathStr = utils.GetFinalPath(pathProyecto, "Shaders/cube.vs");
	const char* cubeVS = vertexpathStr.c_str();

	string fragmentPathString = utils.GetFinalPath(pathProyecto, "Shaders/cube.fs");
	const char* cubeFS = fragmentPathString.c_str();
	///////////////////////////////////////////////////////////////////////////////
	string vertexpathDepthStr = utils.GetFinalPath(pathProyecto, "Shaders/depth.vs");
	const char* depthVS = vertexpathDepthStr.c_str();

	string fragmentPathDepthString = utils.GetFinalPath(pathProyecto, "Shaders/depth.fs");
	const char* depthFS = fragmentPathDepthString.c_str();
	///////////////////////////////////////////////////////////////////////////////////////////////
	string vertexpathDebugStr = utils.GetFinalPath(pathProyecto, "Shaders/debug.vs");
	const char* debugVS = vertexpathDebugStr.c_str();

	string fragmentPathDebugString = utils.GetFinalPath(pathProyecto, "Shaders/debug.fs");
	const char* debugFS = fragmentPathDebugString.c_str();
	///////////////////////////////////////////////////////////////////////////////////////////////

	string vertexpathlighthStr = utils.GetFinalPath(pathProyecto, "Shaders/light.vs");
	const char* lightVS = vertexpathlighthStr.c_str();

	string fragmentPathlighthStr = utils.GetFinalPath(pathProyecto, "Shaders/light.fs");
	const char* lightFS = fragmentPathlighthStr.c_str();
	///////////////////////////////////////////////////////////////////////////////////////////////
	string pathFinalImagen1String = utils.GetFinalPath(pathProyecto, "Textures/albedo.png");
	const char* tex1Path = pathFinalImagen1String.c_str();

	string pathFinalImagen2String = utils.GetFinalPath(pathProyecto, "Textures/albedo.png");
	const char* tex2Path = pathFinalImagen2String.c_str();
	//////////////////////////////////////////////////////////////////////////////////////////////
	uint32_t text1 = createTexture(tex1Path, true);
	uint32_t text2 = createTexture(tex2Path, true);



	Shader shader = Shader(cubeVS, cubeFS);

	Shader depthShader = Shader(depthVS, depthFS);
	Shader debugShader = Shader(debugVS, debugFS);
	Shader lightShader = Shader(lightVS, lightFS);

	auto fboRes = createFBO();

	uint32_t VAOCubo =		 createVertexData(verticesCubo, 24, indicesCubo, 36);
	uint32_t VAOQuad =		 createVertexData(quad_vertices_suelo, 6, quad_indices, 6);
	uint32_t VAOScreenQuad = createVertexData(quad_screen_vertices, 6, indicesQuadScreeen, 6);


	//Bucle inicial donde se realiza toda la accion del motor
	while (!glfwWindowShouldClose(window.GetWindow())) {
		float currentFrame = glfwGetTime();
		float deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		HandlerInput(window.GetWindow(), deltaTime);
		window.HandlerInput();

		Render(shader, depthShader, debugShader, lightShader, VAOCubo, VAOQuad, VAOScreenQuad, text1, text2, fboRes.first, fboRes.second);

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

