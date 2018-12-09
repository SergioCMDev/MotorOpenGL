#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<iostream>
#include<cstdint>
#include<stdio.h>

#include "Shader.h"
#include "Utils.h"
#include "Window.h"
#include "Buffer.h"
#include "Texture.h"

Utils utils;



using namespace std;
Window window;

const int widht = 800, height = 600;
const char* pathProyecto = "../tests/EJ4_5/";
uint32_t elementsVertexs = 20;

float vertexs[]{
	// Position					// UVs
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,	//Front	
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f };


uint32_t elementsIndexes = 6;

uint32_t indexes[]{
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
	//glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//cuando la ventana cambie de tamaño
	glfwSetFramebufferSizeCallback(window.GetWindow(), OnChangeFrameBufferSize);
	return 1;
};

void ChangeInterpolationUniform(Shader& shader, char* uniformName, float value) {

	int idProgram = shader.GetIdProgram();
	//si es -1 es error
	int vertexPositionLocation = glGetUniformLocation(idProgram, uniformName);
	if (vertexPositionLocation < 0) {
		cout << "Error al cargar Uniform " << uniformName << endl;
	}

	glUniform1f(vertexPositionLocation, value);
}


void Render(uint32_t VAO, const Shader& shader, const uint32_t numberOfElements, uint32_t texture1, uint32_t texture2) {
	//Renderizamos la pantalla con un color basandonos en el esquema RGBA(transparencia)
	//Si lo quitamos, no borra nunca la pantalla
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shader.Use();
	glActiveTexture(GL_TEXTURE0);	glBindTexture(GL_TEXTURE_2D, texture1);	glActiveTexture(GL_TEXTURE1);	glBindTexture(GL_TEXTURE_2D, texture2);
	//Bindeamos VAO
	glBindVertexArray(VAO);

	shader.Set("texture1", 0);
	shader.Set("texture2", 1);

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

	string textureString1 = utils.GetFinalPath(pathProyecto, "Textures/texture1.jpg");
	const char* pathFinalImagen1 = textureString1.c_str();

	string textureString2 = utils.GetFinalPath(pathProyecto, "Textures/texture2.jpg");
	const char* pathFinalImagen2 = textureString2.c_str();

	Shader shader = Shader(vertexpath, fragmentPath1);
	int program = shader.GetIdProgram();
	uint32_t VBOFigura, EBO;

	long sizeOfIndices, sizeOfVertices;

	sizeOfIndices = elementsIndexes * sizeof(float);
	sizeOfVertices = elementsVertexs * sizeof(float);


	//float verticesQuad = cuadrado.GetVertexs();
	Buffer buffer = Buffer(sizeOfIndices, sizeOfVertices);
	buffer.SetStatusVerticesColor(false);
	buffer.SetStatusVerticesTextura(true);
	buffer.SetStatusVerticesNormal(false);
	buffer.SetSizeVerticesTextura(2);
	uint32_t numberOfElementsToDraw = buffer.GetElementsToDraw();

	uint32_t VAO = buffer.CreateVAO(&VBOFigura, &EBO, indexes, sizeOfIndices, vertexs,
		sizeOfVertices, &shader);


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

		ChangeInterpolationUniform(shader, "interpolationValue", interpolationValue);
		Render(VAO, shader, numberOfElementsToDraw, image1.GetTexture(), image2.GetTexture());

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

