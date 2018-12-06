#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<iostream>
#include<cstdint>
#include<stdio.h>

#include "Shader.h"
#include "Renderer.h"
#include "Utils.h"
#include "Figure.h"
#include "Window.h"
#include "Buffer.h"
#include "Texture.h"

Utils utils;
glm::vec3 cubePositions[] = {
 glm::vec3(0.0f, 0.0f, 0.0f),
 glm::vec3(2.0f, 5.0f, -15.0f),
 glm::vec3(-1.5f, -2.2f, -2.5f),
 glm::vec3(-3.8f, -2.0f, -12.3f),
 glm::vec3(2.4f, -0.4f, -3.5f),
 glm::vec3(-1.7f, 3.0f, -7.5f),
 glm::vec3(1.3f, -2.0f, -2.5f),
 glm::vec3(1.5f, 2.0f, -2.5f),
 glm::vec3(1.5f, 0.2f, -1.5f),
 glm::vec3(-1.3f, 1.0f, -1.5f)
};


using namespace std;
Window window;

const int widht = 800, height = 600;
const char* pathProyecto = "../tests/AG05_1/";
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
	//glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//cuando la ventana cambie de tamaño
	glfwSetFramebufferSizeCallback(window.GetWindow(), OnChangeFrameBufferSize);
	return 1;
};




int main(int argc, char* argv[]) {
	if (!Inicializacion()) {
		return -1;
	}
	Renderer render;
	const char* vertexpath = utils.GetFinalPath(pathProyecto, "Shaders/vertex.vs");
	const char* fragmentPath1 = utils.GetFinalPath(pathProyecto, "Shaders/fragment.fs");

	Shader shader = Shader(vertexpath, fragmentPath1);
	int program = shader.GetIdProgram();
	uint32_t VBOFigura, EBO;

	Figure cuadrado = Figure(Figuras::Cubo);

	uint32_t* indicesQuad = cuadrado.GetIndexs();
	float* verticesQuad = cuadrado.GetVertexs();

	long sizeOfIndices, sizeOfVertices;

	sizeOfIndices = cuadrado.GetNumberOfElementsIndexs() * sizeof(float);
	sizeOfVertices = cuadrado.GetNumberOfElementsVertexs() * sizeof(float);


	//float verticesQuad = cuadrado.GetVertexs();
	Buffer buffer = Buffer(sizeOfIndices, sizeOfVertices);
	buffer.SetStatusVerticesColor(false);
	buffer.SetStatusVerticesTextura(true);
	buffer.SetSizeVerticesTextura(2);
	uint32_t numberOfElementsToDraw = buffer.GetElementsToDraw();

	//uint32_t VAO = buffer.CreateVAO(&VBOFigura, &EBO, indicesQuad, verticesQuad, &shader);
	uint32_t elementsPerLine = 5;
	uint32_t VAO = buffer.CreateVAO(&VBOFigura, &EBO, indicesQuad, sizeOfIndices, verticesQuad,
		sizeOfVertices, &shader, &elementsPerLine);

	char* pathFinalImagen1 = utils.GetFinalPath(pathProyecto, "Textures/texture1.jpg");
	char* pathFinalImagen2 = utils.GetFinalPath(pathProyecto, "Textures/texture2.jpg");

	Texture image1 = Texture(pathFinalImagen1, 1024, 1024, 1, 0, true);
	image1.LoadTexture();
	Texture image2 = Texture(pathFinalImagen2, 1024, 1024, 1, 0, true);
	image2.LoadTexture();


	float interpolationValue = 0.6;
	//Bucle inicial donde se realiza toda la accion del motor
	while (!glfwWindowShouldClose(window.GetWindow())) {
		window.HandlerInput();
		//Si pulsamos 0 añade interpolacion
		if (window.GetButtonMoreShiny()) {
			interpolationValue += 0.1;
		}
		//Si pulsamos 1 quita interpolacion
		if (window.GetButtonLessShiny()) {
			interpolationValue -= 0.1;
		}
		render.ChangeInterpolationUniform(shader, "interpolationValue", interpolationValue);
		render.Render(VAO, shader, numberOfElementsToDraw, image1.GetTexture(), image2.GetTexture(), true, 10, cubePositions);

		glfwSwapBuffers(window.GetWindow());
		glfwPollEvents();
	}

	//Si se han linkado bien los shaders, los borramos ya que estan linkados
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBOFigura);
	glDeleteBuffers(1, &EBO);
	image1.ReleaseTexture();
	image2.ReleaseTexture();

	glfwTerminate();
	return 0;
}
#pragma endregion

