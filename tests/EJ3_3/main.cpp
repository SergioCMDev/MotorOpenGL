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
const char* pathProyecto = "../tests/EJ3_3/";
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
	const char* vertexpath = utils.GetFinalPath(pathProyecto, "Shaders/vertex.vs");
	const char* fragmentPath1 = utils.GetFinalPath(pathProyecto, "Shaders/fragment.fs");

	Shader shader1 = Shader(vertexpath, fragmentPath1);
	int program = shader1.GetIdProgram();
	uint32_t VBOTriangulo1, EBO;
	uint32_t indicesHexagono[] = {
		2,1,0
	};
	float vertices1[] = {
		//Color
-0.2f, 0.1f, 0.0f,		1.0f, 0.0f,0.0f,
0.2f, 0.1f, 0.0f,		1.0f, 0.0f,0.0f,
0.0f, -0.2f, 0.0f,		1.0f, 0.0f,0.0f

	};

	 Buffer buffer = Buffer();
	uint32_t sizeOfIndices = sizeof(indicesHexagono); //3 uint32_t * sizeofuint32_t(4) = 12
	uint32_t sizeOfVertices = sizeof(vertices1);  //42 floats * sizeoffloat(4) = 168
	uint32_t numberOfElements = sizeof(indicesHexagono) / sizeof(float); //72 vertices / sizeoffloat(4) = 18
	//El VAO Agrupa todos los VBO y EBO
	uint32_t VAOTriangules = buffer.CreateVAO(&VBOTriangulo1, &EBO, indicesHexagono, sizeOfIndices, vertices1,
		sizeOfVertices, &shader1);

	//Bucle inicial donde se realiza toda la accion del motor
	while (!glfwWindowShouldClose(window.GetWindow())) {
		window.HandlerInput();

		render.ChangePosicionUniform(shader1, "nuevaPosUniform");
		render.CambiarColorUniform(shader1, "myColorUniform");
		render.Render(VAOTriangules, shader1, numberOfElements);
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

