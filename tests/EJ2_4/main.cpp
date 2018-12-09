#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<iostream>
#include<cstdint>
#include<stdio.h>
#include "Shader.h"
#include "Utils.h"

using namespace std;

GLFWwindow* window;
Utils utils;
const char* pathProyecto = "../tests/EJ2_4/";
#pragma region Cabezeras

void Render(GLfloat R, GLfloat G, GLfloat B, GLfloat A);
void Render();
void HandlerInput(GLFWwindow* window);
void OnChangeFrameBufferSize(GLFWwindow* window, const int32_t width, const int32_t height);

uint32_t indicesTriangulo1[] = { 0, 2, 1 };
uint32_t indicesTriangulo2[] = { 3, 4, 5 };
float vertices1[] = {
	//Primer triangulo			//Color
	 -0.4f, 0.0f, 0.0f,			//1.0f, 0.0f, 0.0f,
	 -0.2f, 0.3f, 0.0f,		  	//0.0f, 1.0f, 0.0f,
	  0.0f, 0.0f, 0.0f,			//0.0f, 0.0f, 1.0f,
	  //Segundo triangulo
	  0.0f, 0.0f, 0.0f,			//1.0f, 0.0f, 1.0f,
	  0.4f, 0.0f, 0.0f,			//1.0f, 0.0f, 1.0f,
	  0.2f, 0.3f, 0.0f,		  	//0.0f, 1.0f, 0.0f,
};
#pragma endregion


#pragma region Metodos

//Devuelve un VAO formado por todos los componentes
uint32_t createvertexDatatriangulo1(uint32_t *VBO, uint32_t *EBO, uint32_t indices[], uint32_t sizeOfIndices,
	float vertices[], uint32_t sizeOfVertices) {
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
	glBufferData(GL_ARRAY_BUFFER, sizeOfVertices, vertices, GL_STATIC_DRAW);

	//Bindeo buffer indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeOfIndices, indices, GL_STATIC_DRAW);

	//vertices del triangulo
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glad_glEnableVertexAttribArray(0);

	////Vertices de color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	////Lo habilitamos
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



int Inicializacion() {
	if (!glfwInit()) {
		cout << "Error initializing GLFW" << endl;
		glfwTerminate();
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(800, 600, "Window 1", NULL, NULL);
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
	return 1;
}


void Render(uint32_t VAO, const Shader& shader, const uint32_t numberOfElements, bool limpiarPantalla) {
	//Renderizamos la pantalla con un color basandonos en el esquema RGBA(transparencia)
	//Si lo quitamos, no borra nunca la pantalla
	if (limpiarPantalla) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	shader.Use();
	glBindVertexArray(VAO);
	//Bindeamos VAO


	glDrawElements(GL_TRIANGLES, numberOfElements, GL_UNSIGNED_INT, 0);
}

int main(int argc, char* argv[]) {
	if (!Inicializacion()) {
		return -1;
	}

	string vertexpathStr = utils.GetFinalPath(pathProyecto, "Shaders/vertex.vs");
	const char* vertexpath = vertexpathStr.c_str();

	string fragmentPathString = utils.GetFinalPath(pathProyecto, "Shaders/fragment1.fs");
	const char* fragmentPath1 = fragmentPathString.c_str();

	string fragmentPathString2 = utils.GetFinalPath(pathProyecto, "Shaders/fragment2.fs");
	const char* fragmentPath2 = fragmentPathString.c_str();

	Shader shader1(vertexpath, fragmentPath1);
	Shader shader2(vertexpath, fragmentPath2);


	uint32_t VBOTriangulo1, EBO;

	uint32_t sizeOfIndices = sizeof(indicesTriangulo1); //3 uint32_t * sizeofuint32_t(4) = 12
	uint32_t sizeOfVertices = sizeof(vertices1);  //18 floats * sizeoffloat(4) = 18
	uint32_t numberOfElements = sizeof(indicesTriangulo1) / sizeof(float); //72 vertices / sizeoffloat(4) = 18

	//El VAO Agrupa todos los VBO y EBO
	uint32_t VAOTriangules = createvertexDatatriangulo1(&VBOTriangulo1, &EBO, indicesTriangulo1, sizeOfIndices, vertices1, sizeOfVertices);
	uint32_t VAOTriangules2 = createvertexDatatriangulo1(&VBOTriangulo1, &EBO, indicesTriangulo2, sizeOfIndices, vertices1, sizeOfVertices);

	//Bucle inicial donde se realiza toda la accion del motor
	while (!glfwWindowShouldClose(window)) {
		HandlerInput(window);
		Render(VAOTriangules, shader1, numberOfElements, false);
		Render(VAOTriangules2, shader2, numberOfElements, false);
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
