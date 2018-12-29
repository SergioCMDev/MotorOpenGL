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

#pragma region Variables Globales

Utils utils;
Camera camera(glm::vec3(0.0f, 0.0f, 2.0f));


float lastFrame = 0.0f;
bool firstMouse = true;

const uint32_t screen_width = 800, screen_height = 600;
float lastX = (float)screen_width / 2.0f;
float lastY = (float)screen_height / 2.0f;
Window window;

using namespace std;

glm::vec3 pointLightPositions[] = {
	glm::vec3(0.7f, 5.2f, 2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f)
};

glm::vec3 spotLightPositions[] = {
	glm::vec3(2.7f, 3.2f, 2.0f),
	glm::vec3(-2.7f, 3.2f, -5.0f),

};
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

#pragma endregion

const char* pathProyecto = "../tests/EJ9_1/";
#pragma region Cabezeras
void OnChangeFrameBufferSize(GLFWwindow* window, const int32_t width, const int32_t height);
#pragma endregion


#pragma region Eventos

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

	float xoffset = xpos - lastX;
	float yoffset = ypos - lastY;
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
#pragma endregion

#pragma region Metodos


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

	//cuando la ventana cambie de tamaño
	glfwSetCursorPosCallback(window.GetWindow(), OnMouse);
	glfwSetFramebufferSizeCallback(window.GetWindow(), OnChangeFrameBufferSize);
	glfwSetScrollCallback(window.GetWindow(), OnScroll);
	glfwSetInputMode(window.GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	return 1;
};


void Render(uint32_t VAO, const Shader& shader, const Shader& shaderCube, const Model& obj) {
	//Renderizamos la pantalla con un color basandonos en el esquema RGBA(transparencia)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 projection = glm::perspective(glm::radians(camera.GetFOV()), (float)screen_width / screen_height, 0.1f, 60.0f);

	shader.Use();
	shader.Set("projection", projection);
	shader.Set("view", view);
	vec3 posObj (0.0f, 0.0f, -5.0f);
	glm::mat4 model(1.0f);
	model = glm::translate(model, posObj);
	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
	shader.Set("model", model);
	glm::mat3 normalMat = glm::inverse(glm::transpose(glm::mat3(model)));
	shader.Set("normalMat", normalMat);
	shader.Set("viewPos", camera.GetPosition());

	//DirectionalLight 1
	shader.Set("dirLight.direction", 0.0f, 0.0f, 0.0f);
	shader.Set("dirLight.ambient", 0.1f, 0.1f, 0.1f);
	shader.Set("dirLight.diffuse", 0.3f, 0.3f, 0.3f);
	shader.Set("dirLight.specular", 0.5f, 0.5f, 0.5f);

	vec3 pointLightPositions(0.0f, 5.0f, 0.0f);
	//PointLight 1
	shader.Set("pointLights.position", pointLightPositions);
	shader.Set("pointLights.ambient", 0.1f, 0.1f, 0.1f);
	shader.Set("pointLights.diffuse", 0.5f, 0.5f, 0.5f);
	shader.Set("pointLights.specular", 1.0f, 1.0f, 1.0f);
	shader.Set("pointLights.constant", 11.0f);
	shader.Set("pointLights.linear", 0.09f);
	shader.Set("pointLights.cuadratic", 0.032f);

	//SpotLight
	shader.Set("spotLights.position", spotLightPositions[0]);
	shader.Set("spotLights.direction", 0.0f, 0.0f, 0.0f);
	shader.Set("spotLights.cutOff", cos(radians(20.0f)));
	shader.Set("spotLights.outerCutOff", cos(radians(25.0f)));
	shader.Set("spotLights.linear", 0.09f);
	shader.Set("spotLights.constant", 1.0f);
	shader.Set("spotLights.cuadratic", 0.032f);
	shader.Set("spotLights.direction", -1.0f, 0.0f, -1.0f);
	shader.Set("spotLights.ambient", 0.2f, 0.15f, 0.1f);
	shader.Set("spotLights.diffuse", 0.5f, 0.5f, 0.5f);
	shader.Set("spotLights.specular", 1.0f, 1.0f, 1.0f);

	shader.Set("material.shininess", 25.6f);

	obj.Draw(shader);

	//shaderCube.Use();
	//shaderCube.Set("projection", projection);
	//shaderCube.Set("view", view);


	//shaderCube.Set("viewPos", camera.GetPosition());
	//glm::mat4 modelLight(1.0f);
	//modelLight = glm::translate(model, pointLightPositions);
	//modelLight = glm::scale(model, glm::vec3(1.1f, 1.1f, 1.1f));
	//shaderCube.Set("model", modelLight);
	//glBindVertexArray(VAO);

	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


	glBindVertexArray(0);

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


	string vertexpathStr = utils.GetFinalPath(pathProyecto, "Shaders/shader.vs");
	const char* vertexpath = vertexpathStr.c_str();

	string fragmentPathString = utils.GetFinalPath(pathProyecto, "Shaders/fragment.fs");
	const char* fragmentPath1 = fragmentPathString.c_str();

	string vertexpathLightString = utils.GetFinalPath(pathProyecto, "Shaders/cube.vs");
	const char* lightVS = vertexpathLightString.c_str();

	string fragmentPathLightString = utils.GetFinalPath(pathProyecto, "Shaders/cube.fs");
	const char* lightFS = fragmentPathLightString.c_str();

	Shader shader = Shader(vertexpath, fragmentPath1);
	Shader shader2 = Shader(lightVS, lightFS);

	Model object("../assets/obj/Grua/Neck_Mech_Walker_by_3DHaupt-(Wavefront OBJ).obj");

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	uint32_t VAO = createVertexData(verticesCubo, numeroElementosVerticesCubo, indicesCubo, numeroIndicesCubo);

	//Bucle inicial donde se realiza toda la accion del motor
	while (!glfwWindowShouldClose(window.GetWindow())) {
		float currentFrame = glfwGetTime();
		float deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		HandlerInput(window.GetWindow(), deltaTime);
		window.HandlerInput();

		Render(VAO, shader, shader2, object);

		glfwSwapBuffers(window.GetWindow());
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}
#pragma endregion

