#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_precision.hpp>

#include<iostream>
#include<cstdint>
#include<stdio.h>
#include "Headers/Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
using namespace std;

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

#pragma region Cabezeras

void Render(GLfloat R, GLfloat G, GLfloat B, GLfloat A);
void Render();
void HandlerInput(GLFWwindow* window);
void OnChangeFrameBufferSize(GLFWwindow* window, const int32_t width, const int32_t height);
#pragma endregion


#pragma region Metodos

//Devuelve un VAO formado por todos los componentes para crear un cubo
uint32_t createvectexData(uint32_t *VBO, uint32_t *EBO) {
	float vertices[] = {
		// Position					// UVs
		-0.5f, -0.5f, 0.5f,			0.0f, 0.0f, //Front
		0.5f, -0.5f, 0.5f,			1.0f, 0.0f,
		0.5f, 0.5f, 0.5f,			1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f,			0.0f, 1.0f,

		0.5f, -0.5f, 0.5f,			0.0f, 0.0f, //Right
		0.5f, -0.5f, -0.5f,			1.0f, 0.0f,
		0.5f, 0.5f, -0.5f,			1.0f, 1.0f,
		0.5f, 0.5f, 0.5f,			0.0f, 1.0f,

		-0.5f, -0.5f, -0.5f,		1.0f, 0.0f, //Back
		-0.5f, 0.5f, -0.5f,			1.0f, 1.0f,
		0.5f, 0.5f, -0.5f,			0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,			0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f,			1.0f, 0.0f, //Left
		-0.5f, 0.5f, 0.5f,			1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f,			0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,		0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f,			0.0f, 1.0f, //Bottom
		-0.5f, -0.5f, -0.5f,		0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,			1.0f, 0.0f,
		0.5f, -0.5f, 0.5f,			1.0f, 1.0f,

		-0.5f, 0.5f, 0.5f,			0.0f, 0.0f, //Top
		0.5f, 0.5f, 0.5f,			1.0f, 0.0f,
		0.5f, 0.5f, -0.5f,			1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f,			0.0f, 1.0f
	};	 //orden de los vertices
	uint32_t indices[] = {
	 0, 1, 2, 0, 2, 3, //Front
	 4, 5, 6, 4, 6, 7, //Right
	 8, 9, 10, 8, 10, 11, //Back
	 12, 13, 14, 12, 14, 15, //Left
	 16, 17, 18, 16, 18, 19, //Bottom
	 20, 21, 22, 20, 22, 23 //Top
	};


	uint32_t VAO;
	glGenVertexArrays(1, &VAO);
	//Generamos 2 buffer, elementos y objetos
	glGenBuffers(1, VBO);
	glGenBuffers(1, EBO);

	//Bindeamos el VAO
	glBindVertexArray(VAO);

	//Bindeamos buffer vertices
	glBindBuffer(GL_ARRAY_BUFFER, *VBO);
	//Subida de vertices al buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Bindeo buffer indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glad_glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	//Lo habilitamos
	glad_glEnableVertexAttribArray(1);

	//desbindeamos buffer objetos
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Desbindeo
	glBindVertexArray(0);

	//desbindeamos buffer elements
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return VAO;
}

uint32_t createTexture(const char* path) {
	uint32_t texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nChannel;
	unsigned char* data = stbi_load(path, &width, &height, &nChannel, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		cout << "Failed to load texture " << path << endl;
	}
	return texture;
}

void Render(uint32_t VAO, const Shader& shader, const uint32_t texture1, const uint32_t texture2) {
	//Renderizamos la pantalla con un color basandonos en el esquema RGBA(transparencia)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shader.Use();
	//Bindeamos VAO
	glBindVertexArray(VAO);

	glBindVertexArray(GL_TEXTURE0);
	//Subimos a GPU
	glBindTexture(GL_TEXTURE_2D, texture1);

	glBindVertexArray(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);

	shader.Set("texture1", 0);
	shader.Set("texture2", 1);

	//generamos una matriz para las texturas


	glm::mat4 view = glm::mat4(1.0f);
	//alejamos el mundo 3
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 10.0f);

	//glm::mat4 transform = glm::mat4(1.0f);
	//transform = glm::rotate(transform, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	//transform = glm::scale(transform, glm::vec3(0.5f, 0.5f, 0.5f));
	shader.Set("view", view);
	shader.Set("projection", projection);

	for (uint32_t i = 0; i < 10; ++i) {

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[i]);
		float angle = 10.0f + (20.0f * i);
		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(0.5f, 1.0f, 0.0f));

		shader.Set("model", model);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	}


}



void Render(uint32_t VAO, const Shader& shader) {
	//Renderizamos la pantalla con un color basandonos en el esquema RGBA(transparencia)
	glClear(GL_COLOR_BUFFER_BIT);
	shader.Use();
	//Bindeamos VAO
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Render(GLfloat R, GLfloat G, GLfloat B, GLfloat A) {
	//Renderizamos la pantalla con un color basandonos en el esquema RGBA(transparencia)
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(R, G, B, A);
}

void Render() {
	//Renderizamos la pantalla con un color basandonos en el esquema RGBA(transparencia)
	glClear(GL_COLOR_BUFFER_BIT);

	Render(0.0f, 0.0f, 1.0f, 0.0f);
}

void Render(uint32_t VAO, uint32_t program) {
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(program);

	//Bindeamos VAO
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

}

void HandlerInput(GLFWwindow* window) {
	//Si pulsamos la tecla ESC cerramos la ventana
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

void OnChangeFrameBufferSize(GLFWwindow* window, const int32_t width, const int32_t height) {
	//redimension de pantalla 
	//Cambio de clip scene a view scene
	glViewport(0, 0, width, height);
}



int main(int argc, char* argv[]) {
	if (!glfwInit()) {
		cout << "Error initializing GLFW" << endl;
		glfwTerminate();
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Window 1", NULL, NULL);
	if (!window) {
		cout << "Error creating window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Error initializing GLAD" << endl;
		return -1;
	}

	//cuando la ventana cambie de tamaño
	glfwSetFramebufferSizeCallback(window, OnChangeFrameBufferSize);
	const char* path = "../tests/AG05_1/";
	char* result = (char*)calloc(strlen(path) + strlen("Shaders/vertex.vs") + 1, sizeof(char));
	char* result2 = (char*)calloc(strlen(path) + strlen("Shaders/fragment.fs") + 1, sizeof(char));
	strcpy(result, path); // copy string one into the result.
	strcpy(result2, path); // copy string one into the result.
	Shader shader(strcat(result, "Shaders/vertex.vs"), strcat(result2, "Shaders/fragment.fs"));
	//Shader shader("../tests/AG04/Shaders/vertex.vs", "../tests/AG05_1/Shaders/fragment.fs");

	uint32_t VBO, EBO;
	//El VAO Agrupa todos los VBO y EBO
	uint32_t VAO = createvectexData(&VBO, &EBO);

	result = (char*)calloc(strlen(path) + strlen("Textures/texture1.jpg") + 1, sizeof(char));
	result2 = (char*)calloc(strlen(path) + strlen("Textures/texture2.jpg") + 1, sizeof(char));
	strcpy(result, path); // copy string one into the result.
	strcpy(result2, path); // copy string one into the result.
	char* resultTexture1 = strcat(result, "Textures/texture1.jpg");
	uint32_t texture1 = createTexture(resultTexture1);
	uint32_t texture2 = createTexture(strcat(result2, "Textures/texture2.jpg"));
	//uint32_t texture1 = createTexture("../tests/AG05_1/Textures/texture1.jpg");
	//uint32_t texture2 = createTexture("../tests/AG05_1/Textures/texture2.jpg");
	//Bucle inicial donde se realiza toda la accion del motor


	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	while (!glfwWindowShouldClose(window)) {
		HandlerInput(window);
		Render(VAO, shader, texture1, texture2);
		//Render(1.0f, 0.0f, 0.0f, 1.0f);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//Crear metodo para esto
//Si se han linkado bien los shaders, los borramos ya que estan linkados
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glDeleteBuffers(1, &texture1);
	glDeleteBuffers(2, &texture2);

	glfwTerminate();
	return 0;
}
#pragma endregion
