#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<iostream>
#include<cstdint>
#include<stdio.h>

#include "Shader.h"
#include "Utils.h"
#include "Window.h"
#include "Camera.h"
#include "string.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Utils utils;
Camera camera(glm::vec3(-1.0f, 2.0f, 3.0f));
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
float lastFrame = 0.0f;
bool firstMouse = true;

const uint32_t screen_width = 800, screen_height = 600;
float lastX = (float)screen_width / 2.0f;
float lastY = (float)screen_height / 2.0f;
Window window;


using namespace std;

const string pathProyecto = "../tests/AG11/";
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
#pragma endregion

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

void Render(uint32_t VAO, 
	const Shader& shader_bump, const Shader& shader_spec,
	const uint32_t textureAlbedo, const uint32_t textureSpecular, const uint32_t textureNormal) {
	//Renderizamos la pantalla con un color basandonos en el esquema RGBA(transparencia)
	//Si lo quitamos, no borra nunca la pantalla

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::mat4 view = camera.GetViewMatrix();

	glm::mat4 projection = glm::perspective(glm::radians(camera.GetFOV()), 800.0f / 600.0f, 0.1f, 10.0f);


	float l_pos[] = { sin((float)glfwGetTime() / 2.0f), 0.0f, abs(cos((float)glfwGetTime() / 2.0f)) };
	lightPos = vec3(l_pos[0], l_pos[1], l_pos[0]);

	//Con shader bump mapping Normal Mapping
	{

	shader_bump.Use();
	shader_bump.Set("projection", projection);
	shader_bump.Set("view", view);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, vec3(1.0f, 0.f, 0.0f));
	glm::mat3 normalMat = glm::inverse(glm::transpose(glm::mat3(model)));

	shader_bump.Set("model", model);
	shader_bump.Set("normalMat", normalMat);

	shader_bump.Set("viewPos", camera.GetPosition());

	shader_bump.Set("light.position", lightPos);
	shader_bump.Set("light.ambient", 0.2f, 0.15f, 0.1f);
	shader_bump.Set("light.diffuse", 0.5f, 0.5f, 0.5f);
	shader_bump.Set("light.specular", 1.0f, 1.0f, 1.0f);

	shader_bump.Set("material.diffuse", 0);
	shader_bump.Set("material.specular", 1);
	shader_bump.Set("material.normal", 2);
	shader_bump.Set("material.shininess", 25.6f);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureAlbedo);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureSpecular);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, textureNormal);

	glBindVertexArray(VAO);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	//Con shader Spec, sin Normal mapping
	{

		shader_spec.Use();
		shader_spec.Set("projection", projection);
		shader_spec.Set("view", view);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, vec3(-1.0f, 0.f, 0.0f));

		glm::mat3 normalMat = glm::inverse(glm::transpose(glm::mat3(model)));

		shader_spec.Set("model", model);
		shader_spec.Set("normalMat", normalMat);

		shader_spec.Set("viewPos", camera.GetPosition());

		shader_spec.Set("light.position", lightPos);
		shader_spec.Set("light.ambient", 0.2f, 0.15f, 0.1f);
		shader_spec.Set("light.diffuse", 0.5f, 0.5f, 0.5f);
		shader_spec.Set("light.specular", 1.0f, 1.0f, 1.0f);

		shader_spec.Set("material.diffuse", 0);
		shader_spec.Set("material.specular", 1);
		shader_spec.Set("material.normal", 2);
		shader_spec.Set("material.shininess", 25.6f);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureAlbedo);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textureSpecular);

		glBindVertexArray(VAO);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
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


uint32_t CreateQuadVertexData() {

	glm::vec3 pos1(-1.0f, 1.0f, 0.0f);
	glm::vec3 pos2(-1.0f, -1.0f, 0.0f);
	glm::vec3 pos3(1.0f, -1.0f, 0.0f);
	glm::vec3 pos4(1.0f, 1.0f, 0.0f);

	glm::vec2 uv1(0.0f, 1.0f);
	glm::vec2 uv2(0.0f, 0.0f);
	glm::vec2 uv3(1.0f, 0.0f);
	glm::vec2 uv4(1.0f, 1.0f);

	glm::vec3 normal(0.0f, 0.0f, 1.0f);

	glm::vec3 tangent1, tangent2, bitangent1, bitangent2;

	glm::vec3 edge1 = pos2 - pos1;
	glm::vec3 edge2 = pos3 - pos1;
	glm::vec2 deltaUV1 = uv2 - uv1;
	glm::vec2 deltaUV2 = uv3 - uv1;

	float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y - edge2.x);
	tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y - edge2.y);
	tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y - edge2.z);
	tangent1 = normalize(tangent1);

	bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x - edge2.x);
	bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x - edge2.y);
	bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x - edge2.z);
	bitangent1 = normalize(bitangent1);

	edge1 = pos3 - pos1;
	edge2 = pos4 - pos1;
	deltaUV1 = uv3 - uv1;
	deltaUV2 = uv4 - uv1;

	f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y - edge2.x);
	tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y - edge2.y);
	tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y - edge2.z);
	tangent2 = normalize(tangent2);

	bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x - edge2.x);
	bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x - edge2.y);
	bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x - edge2.z);
	bitangent2 = normalize(bitangent2);


	float vertices[] = {
		pos1.x, pos1.y, pos1.z, normal.x, normal.y, normal.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
		pos2.x, pos2.y, pos2.z, normal.x, normal.y, normal.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
		pos3.x, pos3.y, pos3.z, normal.x, normal.y, normal.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,

		pos1.x, pos1.y, pos1.z, normal.x, normal.y, normal.z, uv1.x, uv1.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
		pos3.x, pos3.y, pos3.z, normal.x, normal.y, normal.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
		pos4.x, pos4.y, pos4.z, normal.x, normal.y, normal.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
	};

	uint32_t indices[] = {
		0,1,2 ,3,4,5
	};

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
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

	//Bindeo buffer indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);

	//vertices del triangulo 6 por que hay 6 elementos hasta el proximo inicio de linea
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Vertices de textura
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Vertices normal
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);


	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));
	glEnableVertexAttribArray(4);


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
	string vertexpathStr = utils.GetFinalPath(pathProyecto, "Shaders/bump.vs");
	const char* bumpVSPath = vertexpathStr.c_str();

	string fragmentPathString = utils.GetFinalPath(pathProyecto, "Shaders/bump.fs");
	const char* bumpFSPath = fragmentPathString.c_str();

	string vertexpathStr2 = utils.GetFinalPath(pathProyecto, "Shaders/cube.vs");
	const char* cubeVSPath = vertexpathStr2.c_str();

	string fragmentPathString2 = utils.GetFinalPath(pathProyecto, "Shaders/cube.fs");
	const char* cubeFSPath = fragmentPathString2.c_str();

	string pathFinalImagen1String = utils.GetFinalPath(pathProyecto, "Textures/albedo.png");
	const char* pathFinalImagen1 = pathFinalImagen1String.c_str();

	string pathFinalImagen2String = utils.GetFinalPath(pathProyecto, "Textures/specular.png");
	const char* pathFinalImagen2 = pathFinalImagen2String.c_str();

	string pathFinalImagen3String = utils.GetFinalPath(pathProyecto, "Textures/normal.png");
	const char* pathFinalImagen3 = pathFinalImagen3String.c_str();

	uint32_t textDiffuse = createTexture(pathFinalImagen1, true);
	uint32_t textureSpecular = createTexture(pathFinalImagen2, true);
	uint32_t textureNormal = createTexture(pathFinalImagen3, true);

	Shader shaderNormal = Shader(bumpVSPath, bumpFSPath);
	Shader shaderSPec = Shader(cubeVSPath, cubeFSPath);


	uint32_t VAO = CreateQuadVertexData();


	//Bucle inicial donde se realiza toda la accion del motor
	while (!glfwWindowShouldClose(window.GetWindow())) {
		float currentFrame = glfwGetTime();
		float deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		HandlerInput(window.GetWindow(), deltaTime);
		window.HandlerInput();
		Render(VAO, shaderNormal, shaderSPec, textDiffuse, textureSpecular, textureNormal);


		glfwSwapBuffers(window.GetWindow());
		glfwPollEvents();
	}

	//Si se han linkado bien los shaders, los borramos ya que estan linkados
	glDeleteVertexArrays(1, &VAO);



	glfwTerminate();
	return 0;
}
#pragma endregion