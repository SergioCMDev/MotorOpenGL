#include<glad/glad.h>
#include<GLFW/glfw3.h>
#define _USE_MATH_DEFINES
#include <math.h>       /* round, floor, ceil, trunc */

#include<iostream>
#include<cstdint>
#include<stdio.h>
#include "Shader.h"
#include "Utils.h"

using namespace std;
GLFWwindow* window;
Utils utils;
const char* pathProyecto = "../tests/EJ2_6/";

uint32_t indicesFigura[] = {
6,0,1,
6,1,2,
6,2,3,
3,4,6,
4,5,6,
6,5,0

};

#pragma region Cabezeras

void Render(GLfloat R, GLfloat G, GLfloat B, GLfloat A);
void Render();
void HandlerInput(GLFWwindow* window);
void OnChangeFrameBufferSize(GLFWwindow* window, const int32_t width, const int32_t height);
void GetVerticesHexagono(float radio, glm::vec3  newVertices[7]);
#pragma endregion


#pragma region Metodos

//Devuelve un VAO formado por todos los componentes
uint32_t createvertexDatatriangulo1(uint32_t indices[], uint32_t sizeOfIndices, vec3 vertices[], uint32_t sizeOfVertices) {
	uint32_t VAO;
	uint32_t VBO, EBO;
	glGenVertexArrays(1, &VAO);
	//Generamos 2 buffer, elementos y objetos
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	//Bindeamos el VAO
	glBindVertexArray(VAO);

	//Bindeamos buffer vertices
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//Subida de vertices al buffer
	glBufferData(GL_ARRAY_BUFFER, sizeOfVertices, vertices, GL_STATIC_DRAW);

	//Bindeo buffer indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeOfIndices, indices, GL_STATIC_DRAW);

	//vertices del triangulo 6 por que hay 6 elementos hasta el proximo inicio de linea
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glad_glEnableVertexAttribArray(0);

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

void Render(uint32_t VAO, const Shader& shader, const uint32_t numberOfElements) {
	//Renderizamos la pantalla con un color basandonos en el esquema RGBA(transparencia)
	//Si lo quitamos, no borra nunca la pantalla

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader.Use();
	glBindVertexArray(VAO);
	//Bindeamos VAO


	glDrawElements(GL_TRIANGLES, numberOfElements, GL_UNSIGNED_INT, 0);
}


vec3 calcularSigPos(vec3 posActual, uint32_t radio) {
	uint32_t parteEntera1 = radio;
	uint32_t parteEntera2 = radio;
	float grados1;
	float grados2;
	uint32_t parteEntera = parteEntera1 * parteEntera2;
	uint32_t parteGrados = grados1 + grados2;
	float parteTotal1 = parteEntera1 * cos(parteGrados);
	float parteTotal2 = parteEntera2 * sin(parteGrados);
	return vec3(parteTotal1, parteTotal2, 0.0f);
}

vec2 TransformarPolarToCartesiano(vec2 ValorPolar) {
	return vec2(
		ValorPolar.x * cos(ValorPolar.y),
		ValorPolar.x * sin(ValorPolar.y)
	);
}

void GetVerticesHexagono(float radio, glm::vec3  newVertices[7])
{
	for (size_t i = 0; i < 6; i++)
	{
		vec2 ValorPolar = vec2(radio, i * (M_PI) / 3);
		vec2 ValorCartesiano = TransformarPolarToCartesiano(ValorPolar);
		newVertices[i + 1] = vec3(ValorCartesiano, 0.0f);
	}
}

int main(int argc, char* argv[]) {
	if (!Inicializacion()) {
		return -1;
	}

	string vertexpathStr = utils.GetFinalPath(pathProyecto, "Shaders/vertex.vs");
	const char* vertexpath = vertexpathStr.c_str();

	string fragmentPathString = utils.GetFinalPath(pathProyecto, "Shaders/fragment.fs");
	const char* fragmentPath1 = fragmentPathString.c_str();

	Shader shader(vertexpath, fragmentPath1);


	float radio = 1.0f;
	vec3 newVertices[7] = {
		vec3(0.0f,0.0f,0.0f), //Centrado en el (0,0)
	};
	GetVerticesHexagono(radio, newVertices);



	uint32_t sizeOfIndices = sizeof(indicesFigura); //3 uint32_t * sizeofuint32_t(4) = 12
	uint32_t numberOfElements = sizeof(indicesFigura) / sizeof(float); //72 vertices / sizeoffloat(4) = 18
	//El VAO Agrupa todos los VBO y EBO
	uint32_t VAOTriangules = createvertexDatatriangulo1(indicesFigura, sizeOfIndices, newVertices, sizeof(newVertices));

	//Bucle inicial donde se realiza toda la accion del motor
	while (!glfwWindowShouldClose(window)) {
		HandlerInput(window);
		shader.Set("color", vec3(1.0f));
		Render(VAOTriangules, shader, numberOfElements);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//Crear metodo para esto
	//Si se han linkado bien los shaders, los borramos ya que estan linkados
	glDeleteVertexArrays(1, &VAOTriangules);
	

	glfwTerminate();
	return 0;
}
#pragma endregion
