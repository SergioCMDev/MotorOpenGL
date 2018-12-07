#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<iostream>
#include<cstdint>
#include<stdio.h>
#include "Shader.h"

using namespace std;
float vertices1[] = {
						 -0.4f, 0.0f, 0.0f,			1.0f, 0.0f, 0.0f,
						 -0.2f, 0.3f, 0.0f,		  	0.0f, 1.0f, 0.0f,
						  0.0f, 0.0f, 0.0f,			0.0f, 0.0f, 1.0f,
						  0.4f, 0.0f, 0.0f,			1.0f, 0.0f, 0.0f,
						  0.2f, 0.3f, 0.0f,		  	0.0f, 1.0f, 0.0f,
};

uint32_t indices[] = { 0,2,1,
					   2,3,4 };


#pragma region Cabezeras

void Render(GLfloat R, GLfloat G, GLfloat B, GLfloat A);
void Render();
void HandlerInput(GLFWwindow* window);
void OnChangeFrameBufferSize(GLFWwindow* window, const int32_t width, const int32_t height);
#pragma endregion


#pragma region Metodos

//Devuelve un VAO formado por todos los componentes
uint32_t createvertexDatatriangulo1(uint32_t *VBO, uint32_t *EBO) {
	//Vertices del triangulo



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
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

	//Bindeo buffer indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//vertices del triangulo
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glad_glEnableVertexAttribArray(0);

	//Vertices de color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
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



void Render(uint32_t VAO, const Shader& shader, const uint32_t numberOfElements) {
	//Renderizamos la pantalla con un color basandonos en el esquema RGBA(transparencia)
	//Si lo quitamos, no borra nunca la pantalla

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader.Use();
	glBindVertexArray(VAO);
	//Bindeamos VAO

	glDrawElements(GL_TRIANGLES, numberOfElements, GL_UNSIGNED_INT, 0);
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

	const char* path = "../tests/AG03/";
	char* result = (char*)calloc(strlen(path) + strlen("Shaders/vertex.vs") + 1, sizeof(char));
	char* result2 = (char*)calloc(strlen(path) + strlen("Shaders/fragment.fs") + 1, sizeof(char));
	strcpy(result, path); // copy string one into the result.
	strcpy(result2, path); // copy string one into the result.

	Shader shader(strcat(result, "Shaders/vertex.vs"), strcat(result2, "Shaders/fragment.fs"));


	uint32_t VBOTriangulo1, EBO;
	//El VAO Agrupa todos los VBO y EBO
	uint32_t VAOTriangules = createvertexDatatriangulo1(&VBOTriangulo1, &EBO);
	//uint32_t numberOfElements = sizeof(vertices1) / sizeof(float); //72 vertices / sizeoffloat(4) = 18

	//Bucle inicial donde se realiza toda la accion del motor
	while (!glfwWindowShouldClose(window)) {
		HandlerInput(window);
		Render(VAOTriangules, shader, 6);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//Crear metodo para esto
//Si se han linkado bien los shaders, los borramos ya que estan linkados
	glDeleteVertexArrays(1, &VAOTriangules);
	glDeleteBuffers(1, &VBOTriangulo1);
	glDeleteBuffers(1, &EBO);


	glfwTerminate();
	return 0;
}
#pragma endregion
