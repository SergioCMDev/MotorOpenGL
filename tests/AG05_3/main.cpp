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
uint32_t _elementsVertexs = 120;


float vertex[]{
	// Position					// UVs
	-0.5f, -0.5f, 0.5f,		 0.0f, 0.0f,	//Front	
		0.5f, -0.5f, 0.5f,		 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f,		1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f,		0.0f, 1.0f,

		0.5f, -0.5f, 0.5f,		0.0f, 0.0f, //Right
		0.5f, -0.5f, -0.5f,		 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f,		1.0f, 1.0f,
		0.5f, 0.5f, 0.5f,		 0.0f, 1.0f,

		-0.5f, -0.5f, -0.5f,		1.0f, 0.0f, //Back
		-0.5f, 0.5f, -0.5f,		 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f,		0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,		 0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f,		 1.0f, 0.0f, //Left
		-0.5f, 0.5f, 0.5f,		1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f,			0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,		0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f,			0.0f, 1.0f, //Bottom
		-0.5f, -0.5f, -0.5f,		 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,			1.0f, 0.0f,
		0.5f, -0.5f, 0.5f,		1.0f, 1.0f,

		-0.5f, 0.5f, 0.5f,		0.0f, 0.0f, //Top
		0.5f, 0.5f, 0.5f,		1.0f, 0.0f,
		0.5f, 0.5f, -0.5f,		1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f,		 0.0f, 1.0f
};

uint32_t elementsIndexes = 36;

uint32_t indexes[]{
	0, 1, 2, 0, 2, 3 //Front
	,4, 5, 6, 4, 6, 7 //Right
	,8, 9, 10, 8, 10, 11 //Back
	,12, 13, 14, 12, 14, 15 //Left
	,16, 17, 18, 16, 18, 19 //Bottom
	,20, 21, 22, 20, 22, 23 //Top
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
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//cuando la ventana cambie de tama�o
	glfwSetFramebufferSizeCallback(window.GetWindow(), OnChangeFrameBufferSize);
	return 1;
};

void Projection3D(const Shader & shader, bool movimiento, uint32_t numeroRepeticionesElemento, glm::vec3 *cubePositions, const uint32_t numberOfElements)
{

	glm::mat4 view = glm::mat4(1.0f);
	//alejamos el mundo 3D
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 10.0f);

	if (numeroRepeticionesElemento > 0) {
		for (uint32_t i = 0; i < numeroRepeticionesElemento; i++) {
			glm::mat4 model = glm::mat4(1.0f);
			glm::vec3 vector = cubePositions[i];
			model = glm::translate(model, vector);
			float angle;
			if (movimiento) {
				angle = 10.0f + (cos(glfwGetTime()) + (sin(glfwGetTime())));
				model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(0.5f, 1.0f, 0.0f));
			}
			shader.Set("model", model);
			glDrawElements(GL_TRIANGLES, numberOfElements, GL_UNSIGNED_INT, 0);
		}
	}
	shader.Set("view", view);
	shader.Set("projection", projection);
}

void Render(uint32_t VAO, const Shader& shader, const uint32_t numberOfElements, uint32_t texture1,
	uint32_t texture2, bool movimiento, uint32_t numeroRepeticionesElementos, glm::vec3 *cubePositions) {
	//Renderizamos la pantalla con un color basandonos en el esquema RGBA(transparencia)
	//Si lo quitamos, no borra nunca la pantalla
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shader.Use();
	glActiveTexture(GL_TEXTURE0);	glBindTexture(GL_TEXTURE_2D, texture1);	glActiveTexture(GL_TEXTURE1);	glBindTexture(GL_TEXTURE_2D, texture2);
	//Bindeamos VAO
	glBindVertexArray(VAO);
	Projection3D(shader, movimiento, numeroRepeticionesElementos, cubePositions, numberOfElements);
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

	string pathFinalImagen1String = utils.GetFinalPath(pathProyecto, "Textures/texture1.jpg");
	const char* pathFinalImagen1 = pathFinalImagen1String.c_str();

	string pathFinalImagen2String = utils.GetFinalPath(pathProyecto, "Textures/texture2.jpg");
	const char* pathFinalImagen2 = pathFinalImagen2String.c_str();

	Shader shader = Shader(vertexpath, fragmentPath1);
	int program = shader.GetIdProgram();
	uint32_t VBOFigura, EBO;



	long sizeOfIndices, sizeOfVertices;

	sizeOfIndices = elementsIndexes * sizeof(float);
	sizeOfVertices = _elementsVertexs * sizeof(float);


	//float verticesQuad = cuadrado.GetVertexs();
	Buffer buffer = Buffer(sizeOfIndices, sizeOfVertices);
	buffer.SetStatusVerticesColor(false);
	buffer.SetStatusVerticesTextura(true);
	buffer.SetSizeVerticesNormal(false);
	buffer.SetSizeVerticesTextura(2);
	uint32_t numberOfElementsToDrawForGeometry = buffer.GetElementsToDraw();

	//uint32_t VAO = buffer.CreateVAO(&VBOFigura, &EBO, indicesQuad, verticesQuad, &shader);
	//uint32_t elementsPerLine = 5; //en caso de cubo con todos las posiciones
	uint32_t VAO = buffer.CreateVAO(&VBOFigura, &EBO, indexes, sizeOfIndices, vertex,
		sizeOfVertices, &shader);

	Texture image1 = Texture(pathFinalImagen1, 1024, 1024, 1, 0, true);
	image1.LoadTexture();
	Texture image2 = Texture(pathFinalImagen2, 1024, 1024, 1, 0, true);
	image2.LoadTexture();


	float interpolationValue = 0.6;
	//Bucle inicial donde se realiza toda la accion del motor
	while (!glfwWindowShouldClose(window.GetWindow())) {
		window.HandlerInput();
		//Si pulsamos 0 a�ade interpolacion
		if (window.GetButtonMoreShiny()) {
			interpolationValue += 0.1;
		}
		//Si pulsamos 1 quita interpolacion
		if (window.GetButtonLessShiny()) {
			interpolationValue -= 0.1;
		}
		Render(VAO, shader, numberOfElementsToDrawForGeometry, image1.GetTexture(), image2.GetTexture(), true, 10, cubePositions);

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

