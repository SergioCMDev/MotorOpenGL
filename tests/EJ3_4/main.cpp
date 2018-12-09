#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<iostream>
#include<cstdint>
#include<stdio.h>
#include "Shader.h"
#include "Utils.h"
#include "Window.h"
#include "Buffer.h"

using namespace std;
Window window;
Utils utils;
const int widht = 800, height = 600;
const char* pathProyecto = "../tests/EJ3_3/";
uint32_t indicesFigura[] = {
	2,1,0
};
float vertices1[] = {
	//Color
-0.2f, 0.1f, 0.0f,		1.0f, 0.0f,0.0f,
0.2f, 0.1f, 0.0f,		1.0f, 0.0f,0.0f,
0.0f, -0.2f, 0.0f,		1.0f, 0.0f,0.0f

};

#pragma region Cabezeras
void OnChangeFrameBufferSize(GLFWwindow* window, const int32_t width, const int32_t height);
#pragma endregion


#pragma region Metodos


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
	window = Window(widht, height);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Error initializing GLAD" << endl;
		return -1;
	}

	//cuando la ventana cambie de tamaño
	glfwSetFramebufferSizeCallback(window.GetWindow(), OnChangeFrameBufferSize);
	return 1;
}

void ChangePosicionUniform(Shader& shader, char* uniformName, float posX, float posY, float posZ) {

	int idProgram = shader.GetIdProgram();
	//si es -1 es error
	int vertexPositionLocation = glGetUniformLocation(idProgram, uniformName);
	if (vertexPositionLocation < 0) {
		cout << "Error al cargar Uniform " << uniformName << endl;
	}

	glUniform3f(vertexPositionLocation, posX, posY, posZ);
}

void ChangePosicionUniformRandom(Shader& shader, char* uniformName) {
	float timeValue = glfwGetTime();
	float posValue1 = sin(timeValue);	float posValue2 = cos(timeValue);
	ChangePosicionUniform(shader, uniformName, posValue1, posValue2, 0);
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

void CambiarColorUniform(Shader& shader, char* uniformName, float colorValue1, float colorValue2, float colorValue3) {
	float timeValue = glfwGetTime();

	int idProgram = shader.GetIdProgram();
	//si es -1 es error
	int vertexColorLocation = glGetUniformLocation(idProgram, uniformName);
	if (vertexColorLocation < 0) {
		cout << "Error al cargar Uniform " << uniformName << endl;
	}

	glUniform3f(vertexColorLocation, colorValue1, colorValue2, colorValue3);
}

void CambiarColorUniformRandom(Shader& shader, char* uniformName) {
	float timeValue = glfwGetTime();
	float colorValue = sin(timeValue);	float colorValue2 = cos(timeValue);	float colorValue3 = sin(timeValue);	CambiarColorUniform(shader, uniformName, colorValue, colorValue2, colorValue3);
}

int main(int argc, char* argv[]) {
	if (!Inicializacion()) {
		return -1;
	}
	string vertexpathStr = utils.GetFinalPath(pathProyecto, "Shaders/vertex.vs");
	const char* vertexpath = vertexpathStr.c_str();

	string fragmentPathString = utils.GetFinalPath(pathProyecto, "Shaders/fragment.fs");
	const char* fragmentPath1 = fragmentPathString.c_str();

	Shader shader1 = Shader(vertexpath, fragmentPath1);
	int program = shader1.GetIdProgram();
	uint32_t VBOTriangulo1, EBO;


	Buffer buffer = Buffer();
	uint32_t sizeOfIndices = sizeof(indicesFigura); //3 uint32_t * sizeofuint32_t(4) = 12
	uint32_t sizeOfVertices = sizeof(vertices1);  //42 floats * sizeoffloat(4) = 168
	uint32_t numberOfElements = sizeof(indicesFigura) / sizeof(float); //72 vertices / sizeoffloat(4) = 18
	//El VAO Agrupa todos los VBO y EBO
	uint32_t VAOTriangules = buffer.CreateVAO(&VBOTriangulo1, &EBO, indicesFigura, sizeOfIndices, vertices1,
		sizeOfVertices, &shader1);

	//Bucle inicial donde se realiza toda la accion del motor
	while (!glfwWindowShouldClose(window.GetWindow())) {
		window.HandlerInput();

		ChangePosicionUniformRandom(shader1, "nuevaPosUniform");
		CambiarColorUniformRandom(shader1, "myColorUniform");
		Render(VAOTriangules, shader1, numberOfElements);
		glfwSwapBuffers(window.GetWindow());
		glfwPollEvents();
	}

	//Si se han linkado bien los shaders, los borramos ya que estan linkados
	glDeleteVertexArrays(1, &VAOTriangules);
	glDeleteBuffers(1, &VBOTriangulo1);
	glDeleteBuffers(1, &EBO);


	glfwTerminate();
	return 0;
}
#pragma endregion

