#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<iostream>
#include<cstdint>
#include<stdio.h>
#include "Shader.h"
#include "Renderer.h"
#include "Utils.h"
#include "Window.h"

using namespace std;

Window window;
Utils utils;
Renderer render;
const int widht = 800, height = 600;
const char* path = "../tests/EJ2_5/";
#pragma region Cabezeras
void OnChangeFrameBufferSize(GLFWwindow* window, const int32_t width, const int32_t height);
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

	//vertices del triangulo 6 por que hay 6 elementos hasta el proximo inicio de linea
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glad_glEnableVertexAttribArray(0);

	//Vertices de color
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

//void OnChangeFrameBufferSize(GLFWwindow* window, const int32_t width, const int32_t height) {
//	//redimension de pantalla 
//	//Cambio de clip scene a view scene
//	glViewport(0, 0, width, height);
//}



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
	//glfwSetFramebufferSizeCallback(window.GetWindow(), OnChangeFrameBufferSize);
	return 1;
}

int main(int argc, char* argv[]) {
	if (!Inicializacion()) {
		return -1;
	}

	const char* vertexpath = utils.GetFinalPath(path, "Shaders/vertex.vs");
	const char* fragmentPath1 = utils.GetFinalPath(path, "Shaders/fragment.fs");

	Shader shader1(vertexpath, fragmentPath1);
	
	uint32_t VBOTriangulo1, EBO;
	uint32_t indicesHexagono[] = {
		6,0,1,
		6,1,2,
		6,2,3,
		3,4,6,
		4,5,6,
		6,5,0
	};
	float vertices1[] = {
		//Color
-0.2f, -0.3f, 0.0f,			1.0f, 0.0f, 0.0f, //0
0.2f, -0.3f, 0.0f,		  	1.0f, 0.0f, 0.0f, //1
0.3f,  0.0f, 0.0f,			1.0f, 0.0f, 0.0f, //2
0.2f,  0.3f, 0.0f,			1.0f, 0.0f, 0.0f, //3
-0.2f,  0.3f, 0.0f,			1.0f, 0.0f, 0.0f, //4
-0.3f,  0.0f, 0.0f,			1.0f, 0.0f, 0.0f, //5
 0.0f,  0.0f, 0.0f,			1.0f, 0.0f, 0.0f  //6
	};


	uint32_t sizeOfIndices = sizeof(indicesHexagono); //3 uint32_t * sizeofuint32_t(4) = 12
	uint32_t sizeOfVertices = sizeof(vertices1);  //42 floats * sizeoffloat(4) = 168
	uint32_t numberOfElements = sizeof(indicesHexagono) / sizeof(float); //72 vertices / sizeoffloat(4) = 18
	//El VAO Agrupa todos los VBO y EBO
	uint32_t VAOTriangules = createvertexDatatriangulo1(&VBOTriangulo1, &EBO, indicesHexagono, sizeOfIndices, vertices1, sizeOfVertices);

	//Bucle inicial donde se realiza toda la accion del motor
	while (!glfwWindowShouldClose(window.GetWindow())) {
		window.HandlerInput();
		render.Render(VAOTriangules, shader1, numberOfElements);
		glfwSwapBuffers(window.GetWindow());
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
