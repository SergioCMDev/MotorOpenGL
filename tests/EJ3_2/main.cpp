#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<iostream>
#include<cstdint>
#include<stdio.h>
#include "Shader.h"
#include "Utils.h"
#include "Window.h"

using namespace std;
Window window;
Utils utils;
const int widht = 800, height = 600;
const char* pathProyecto = "../tests/EJ3_2/";

uint32_t indicesFigura[] = {
	2,1,0
};
float vertices1[] = {
	//Color
-0.2f, 0.1f, 0.0f,
0.2f, 0.1f, 0.0f,
0.0f,  -0.2f, 0.0f

};


#pragma region Cabezeras
void OnChangeFrameBufferSize(GLFWwindow* window, const int32_t width, const int32_t height);
#pragma endregion


#pragma region Metodos

//Devuelve un VAO formado por todos los componentes
uint32_t createvertexDataFigura(uint32_t *VBO, uint32_t *EBO, uint32_t indices[], uint32_t sizeOfIndices,
	float vertices[], uint32_t sizeOfVertices, Shader* shader) {
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

	//vertices del triangulo 6 por que hay 6 elementos hasta el proximo inicio de linea
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glad_glEnableVertexAttribArray(0);

	//Vertices de color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	//////Lo habilitamos
	glad_glEnableVertexAttribArray(1);

	//desbindeamos buffer objetos
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Desbindeo
	glBindVertexArray(0);

	//desbindeamos buffer elements
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return VAO;
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
	window = Window(widht, height);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Error initializing GLAD" << endl;
		return -1;
	}

	//cuando la ventana cambie de tamaño
	glfwSetFramebufferSizeCallback(window.GetWindow(), OnChangeFrameBufferSize);
	return 1;
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



	uint32_t sizeOfIndices = sizeof(indicesFigura); //3 uint32_t * sizeofuint32_t(4) = 12
	uint32_t sizeOfVertices = sizeof(vertices1);  //42 floats * sizeoffloat(4) = 168
	uint32_t numberOfElements = sizeof(indicesFigura) / sizeof(float); //72 vertices / sizeoffloat(4) = 18
	//El VAO Agrupa todos los VBO y EBO
	uint32_t VAOTriangules = createvertexDataFigura(&VBOTriangulo1, &EBO, indicesFigura, sizeOfIndices, vertices1, sizeOfVertices, &shader1);

	//Bucle inicial donde se realiza toda la accion del motor
	while (!glfwWindowShouldClose(window.GetWindow())) {
		window.HandlerInput();
		CambiarColorUniformRandom(shader1, "myColor");
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

