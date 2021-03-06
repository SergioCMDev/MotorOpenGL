#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<iostream>
#include<cstdint>
#include<stdio.h>

#include "Shader.h"
#include "Utils.h"
#include "Window.h"
#include "Buffer.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Utils utils;

using namespace std;
Window window;

const int widht = 800, height = 600;
const char* pathProyecto = "../tests/EJ4_4/";

uint32_t elementsVertexs = 20;

float vertexs[]{
	// Position					// UVs
	-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,	//Front	
	 0.5f, -0.5f, 0.0f,		0.5f, 0.0f,
	 0.5f, 0.5f, 0.0f,		0.5f, 0.5f,
	-0.5f, 0.5f, 0.0f,		0.0f, 0.5f 
};


uint32_t numeroIndicesCubo = 6;

uint32_t indicesQuadFrontal[]{
0, 1, 2, 0, 2, 3 };

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
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//cuando la ventana cambie de tama�o
	glfwSetFramebufferSizeCallback(window.GetWindow(), OnChangeFrameBufferSize);
	return 1;
};




void Render(uint32_t VAO, const Shader& shader, const uint32_t numberOfElements, uint32_t texture1) {
	//Renderizamos la pantalla con un color basandonos en el esquema RGBA(transparencia)
	//Si lo quitamos, no borra nunca la pantalla
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shader.Use();
	glActiveTexture(GL_TEXTURE0);	glBindTexture(GL_TEXTURE_2D, texture1);
	//Bindeamos VAO
	glBindVertexArray(VAO);

	shader.Set("texture1", 0);

	glDrawElements(GL_TRIANGLES, numberOfElements, GL_UNSIGNED_INT, 0);
}

uint32_t createTexture(const char* path, bool flip) {
	uint32_t texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
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

uint32_t createVertexData(const float* vertices, const uint32_t n_verts, const uint32_t* indices, const uint32_t n_indices) {
	unsigned int VAO, VBO, EBO;

	glGenVertexArrays(1, &VAO);
	//Generamos 2 buffer, elementos y objetos
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	//Bindeamos el VAO
	glBindVertexArray(VAO);

	uint32_t _numberOfElementsPerLine = 5;

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
	string vertexpathStr = utils.GetFinalPath(pathProyecto, "Shaders/vertex.vs");
	const char* vertexpath = vertexpathStr.c_str();

	string fragmentPathString = utils.GetFinalPath(pathProyecto, "Shaders/fragment.fs");
	const char* fragmentPath1 = fragmentPathString.c_str();

	string textureString1 = utils.GetFinalPath(pathProyecto, "Textures/texture2.jpg");
	const char* pathFinalImagen1 = textureString1.c_str();

	Shader shader = Shader(vertexpath, fragmentPath1);
	int program = shader.GetIdProgram();
	uint32_t VBOFigura, EBO;


	long sizeOfIndices, sizeOfVertices;

	sizeOfIndices = numeroIndicesCubo * sizeof(float);
	sizeOfVertices = elementsVertexs * sizeof(float);


	//float verticesQuad = cuadrado.GetVertexs();
	Buffer buffer = Buffer(sizeOfIndices, sizeOfVertices);

	uint32_t numberOfElementsToDraw = buffer.GetElementsToDraw();


	uint32_t VAO = createVertexData(vertexs, elementsVertexs, indicesQuadFrontal, numeroIndicesCubo);
	

	uint32_t texture1 = createTexture(pathFinalImagen1, true);


	//Bucle inicial donde se realiza toda la accion del motor
	while (!glfwWindowShouldClose(window.GetWindow())) {
		window.HandlerInput();

		Render(VAO, shader, numberOfElementsToDraw, texture1);

		glfwSwapBuffers(window.GetWindow());
		glfwPollEvents();
	}

	//Si se han linkado bien los shaders, los borramos ya que estan linkados
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBOFigura);
	glDeleteBuffers(1, &EBO);


	glfwTerminate();
	return 0;
}
#pragma endregion

