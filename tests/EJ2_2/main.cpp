#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<iostream>
#include<cstdint>
#include<stdio.h>
#include "Shader.h"
#include "Utils.h"

const char* pathProyecto = "../tests/EJ2_2/";
using namespace std;
const float verticesTriangulo1[] = {
						 -0.4f, 0.0f, 0.0f,			1.0f, 0.0f, 0.0f,
						 -0.2f, 0.3f, 0.0f,		  	0.0f, 1.0f, 0.0f,
						  0.0f, 0.0f, 0.0f,			0.0f, 0.0f, 1.0f
};



const uint32_t indicesTriangulos[] = { 0,2,1 };


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

	float vertices1[] = {
							 -0.4f, 0.0f, 0.0f,			1.0f, 0.0f, 0.0f,
							 -0.2f, 0.3f, 0.0f,		  	0.0f, 1.0f, 0.0f,
							  0.0f, 0.0f, 0.0f,			0.0f, 0.0f, 1.0f
	};

	uint32_t indices[] = { 0,2,1 };

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

uint32_t createvertexDatatriangulo2(uint32_t *VBO, uint32_t *EBO) {
	//uint32_t createvertexData(uint32_t *VBO, uint32_t *EBO) {
		//Vertices del triangulo

	const float verticesTriangulo2[] = { 0.0f, 0.0f, 0.0f,	1.0f, 0.0f, 0.0f,
								   0.2f, 0.3f, 0.0f,	0.0f, 1.0f, 0.0f,
								   0.4f, 0.0f,0.0f,		0.0f, 0.0f, 1.0f };

	uint32_t indices[] = { 0,1,2 };

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
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesTriangulo2), verticesTriangulo2, GL_STATIC_DRAW);

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

void Render(uint32_t VAO, const Shader& shader) {
	//Renderizamos la pantalla con un color basandonos en el esquema RGBA(transparencia)
	//glClear(GL_COLOR_BUFFER_BIT);
	shader.Use();
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

	//cuando la ventana cambie de tama�o
	glfwSetFramebufferSizeCallback(window, OnChangeFrameBufferSize);

	Utils utils;

	string vertexpathStr = utils.GetFinalPath(pathProyecto, "Shaders/vertex.vs");
	const char* vertexpath = vertexpathStr.c_str();

	string fragmentPathString = utils.GetFinalPath(pathProyecto, "Shaders/fragment.fs");
	const char* fragmentPath1 = fragmentPathString.c_str();

	Shader shader(vertexpath, fragmentPath1);


	uint32_t VBOTriangulo1, VBOTriangulo2, EBO;
	//El VAO Agrupa todos los VBO y EBO
	uint32_t VAOTriangule1 = createvertexDatatriangulo1(&VBOTriangulo1, &EBO);
	uint32_t VAOTriangule2 = createvertexDatatriangulo2(&VBOTriangulo2, &EBO);

	//Bucle inicial donde se realiza toda la accion del motor
	while (!glfwWindowShouldClose(window)) {
		HandlerInput(window);
		Render(VAOTriangule1, shader);
		Render(VAOTriangule2, shader);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//Crear metodo para esto
//Si se han linkado bien los shaders, los borramos ya que estan linkados
	glDeleteVertexArrays(1, &VAOTriangule1);
	glDeleteVertexArrays(1, &VAOTriangule2);
	glDeleteBuffers(1, &VBOTriangulo1);
	glDeleteBuffers(1, &VBOTriangulo2);
	glDeleteBuffers(1, &EBO);


	glfwTerminate();
	return 0;
}
#pragma endregion
