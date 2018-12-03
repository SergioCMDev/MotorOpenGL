#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<iostream>
#include<cstdint>
#include<stdio.h>
#include "Shader.h"
#include "Renderer.h"
#include "Utils.h"
#include "Window.h"
#include "Buffer.h"

using namespace std;
Window window;
Utils utils;
//Renderer render;
const int widht = 800, height = 600;
const char* path = "../tests/EJ4_1/";
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

int main(int argc, char* argv[]) {
	if (!Inicializacion()) {
		return -1;
	}
	Renderer render;
	const char* vertexpath = utils.GetFinalPath(path, "Shaders/vertex.vs");
	const char* fragmentPath1 = utils.GetFinalPath(path, "Shaders/fragment.fs");

	Shader shader = Shader(vertexpath, fragmentPath1);
	int program = shader.GetIdProgram();
	uint32_t VBOTriangulo1, EBO;

	//float verticesQuad[] = {
	//	// Position // UVs
	//	0.5f, 0.5f, 0.0f,		1.0f, 1.0f,
	//	0.5f, -0.5f, 0.0f,		1.0f, 0.0f,
	//	-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,
	//	-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,

	//	//0.5f, -0.5f, 0.5f, 0.0f, 0.0f, //Right
	//	//0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
	//	//0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
	//	//0.5f, 0.5f, 0.5f, 0.0f, 1.0f,

	//	//-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, //Back
	//	//-0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
	//	//0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
	//	//0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

	//	//-0.5f, -0.5f, 0.5f, 1.0f, 0.0f, //Left
	//	//-0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
	//	//-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
	//	//-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

	//	//-0.5f, -0.5f, 0.5f, 0.0f, 1.0f, //Bottom
	//	//-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
	//	//0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
	//	//0.5f, -0.5f, 0.5f, 1.0f, 1.0f,

	//	//-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, //Top
	//	//0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
	//	//0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
	//	//-0.5f, 0.5f, -0.5f, 0.0f, 1.0f
	//};

	float verticesQuad[] = { 0.5f,  0.5f,  0.0f,	1.0f, 0.0f, 0.0f,		1.0f, 10.f,
							 0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,		1.0f, 0.0f,
							-0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f,		0.0f, 0.0f,
							-0.5f,  0.5f, 0.0f,		1.0f, 1.0f, 1.0f,		0.0f, 1.0f };
	uint32_t indicesQuad[] = { 0, 3, 1,
							  1, 3, 2 };

	//uint32_t indicesQuad[] = {
	//	1, 2, 0,3 //Front
	//	//4, 5, 6, 4, 6, 7, //Right
	//	//8, 9, 10, 8, 10, 11, //Back
	//	//12, 13, 14, 12, 14, 15, //Left
	//	//16, 17, 18, 16, 18, 19, //Bottom
	//	//20, 21, 22, 20, 22, 23 //Top
	//};

	Buffer buffer = Buffer(sizeof(indicesQuad), sizeof(verticesQuad));

	uint32_t numberOfElementsPerLine = buffer.GetElementsPerLine();

	uint32_t VAO = buffer.CreateVAO(&VBOTriangulo1, &EBO, indicesQuad, verticesQuad, &shader);



	//Bucle inicial donde se realiza toda la accion del motor
	while (!glfwWindowShouldClose(window.GetWindow())) {
		window.HandlerInput();

		//render.ChangePosicionUniform(shader1, "nuevaPosUniform");
		//render.CambiarColorUniform(shader1, "myColorUniform");
		render.Render(VAO, shader, numberOfElementsPerLine);
		glfwSwapBuffers(window.GetWindow());
		glfwPollEvents();
	}

	//Si se han linkado bien los shaders, los borramos ya que estan linkados
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBOTriangulo1);
	glDeleteBuffers(1, &EBO);


	glfwTerminate();
	return 0;
}
#pragma endregion

