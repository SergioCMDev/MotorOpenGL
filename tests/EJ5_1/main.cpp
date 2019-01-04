#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<iostream>
#include<cstdint>
#include<stdio.h>

#include <Shader.h>
#include <Utils.h>
#include <Window.h>
#include <Buffer.h>
#include "Camera.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastFrame = 0.0f;
bool firstMouse = true;

const uint32_t screen_width = 800, screen_height = 600;
float lastX = (float)screen_width / 2.0f;
float lastY = (float)screen_height / 2.0f;
Window window;
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
uint32_t numeroElementosVerticesCubo = 120;

float verticesCubo[]{
	// Position					// UVs
	-0.5f, -0.5f, 0.5f,	 0.0f, 0.0f,	//Front	
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,

		0.5f, -0.5f, 0.5f, 0.0f, 0.0f, //Right
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f,

		-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, //Back
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f, 1.0f, 0.0f, //Left
		-0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f, 0.0f, 1.0f, //Bottom
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 1.0f,

		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, //Top
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f
};

uint32_t numeroIndicesCubo = 36;

uint32_t indicesCubo[]{
	0, 1, 2, 0, 2, 3 //Front
	,4, 5, 6, 4, 6, 7 //Right
	,8, 9, 10, 8, 10, 11 //Back
	,12, 13, 14, 12, 14, 15 //Left
	,16, 17, 18, 16, 18, 19 //Bottom
	,20, 21, 22, 20, 22, 23 //Top
};

using namespace std;

const char* pathProyecto = "../tests/EJ5_1/";
#pragma region Cabezeras
//void OnChangeFrameBufferSize(GLFWwindow* window, const int32_t width, const int32_t height);
#pragma endregion


#pragma region Metodos

void OnChangeFrameBufferSize(GLFWwindow* window, const int32_t width, const int32_t height) {
	//redimension de pantalla 
	//Cambio de clip scene a view scene
	glViewport(0, 0, width, height);
}

void OnMouse(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse) {
		firstMouse = false;
		lastX = xpos;
		lastY = ypos;
	}

	double xoffset = xpos - lastX;
	double yoffset = ypos - lastY;
	lastX = xpos;
	lastY = ypos;
	camera.handleMouseMovement(xoffset, yoffset);
}


void OnScroll(GLFWwindow* window, double xoffset, double yoffset) {
	camera.handleMouseScroll(yoffset);
}

void HandlerInput(GLFWwindow* window, const double deltaTime) {
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.HandleKeyboard(Camera::Movement::Forward, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.HandleKeyboard(Camera::Movement::Backward, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.HandleKeyboard(Camera::Movement::Left, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.HandleKeyboard(Camera::Movement::Right, deltaTime);
	}
}




int Inicializacion() {
	if (!glfwInit()) {
		cout << "Error initializing GLFW" << endl;
		glfwTerminate();
		return -1;
	}

	window = Window(screen_width, screen_height);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Error initializing GLAD" << endl;
		return -1;
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//cuando la ventana cambie de tamaño
	glfwSetCursorPosCallback(window.GetWindow(), OnMouse);
	glfwSetFramebufferSizeCallback(window.GetWindow(), OnChangeFrameBufferSize);
	glfwSetScrollCallback(window.GetWindow(), OnScroll);
	glfwSetInputMode(window.GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	return 1;
};

void Projection3D(const Shader & shader, uint32_t numeroRepeticionesElemento, glm::vec3 *cubePositions, Camera camera, uint32_t numberOfElements)
{


	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(camera.GetFOV()), 800.0f / 600.0f, 0.1f, 10.0f);

	if (numeroRepeticionesElemento > 0) {
		for (uint32_t i = 0; i < numeroRepeticionesElemento; i++) {
			glm::mat4 model = glm::mat4(1.0f);
			glm::vec3 vector = cubePositions[i];
			model = glm::translate(model, vector);
			float angle;
			angle = 10.0f + (cos(glfwGetTime()) + (sin(glfwGetTime())));
			model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(0.5f, 1.0f, 0.0f));
			shader.Set("model", model);
			glDrawElements(GL_TRIANGLES, numberOfElements, GL_UNSIGNED_INT, 0);
		}
	}
	shader.Set("view", camera.GetViewMatrix());
	shader.Set("projection", projection);
}

void Render(uint32_t VAO, const Shader& shader, const uint32_t numberOfElements, uint32_t texture1,
	uint32_t texture2, uint32_t numeroRepeticionesElementos, glm::vec3 *cubePositions, Camera camera) {
	//Renderizamos la pantalla con un color basandonos en el esquema RGBA(transparencia)
	//Si lo quitamos, no borra nunca la pantalla
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	shader.Use();
	glActiveTexture(GL_TEXTURE0);	glBindTexture(GL_TEXTURE_2D, texture1);	glActiveTexture(GL_TEXTURE1);	glBindTexture(GL_TEXTURE_2D, texture2);
	//Bindeamos VAO
	glBindVertexArray(VAO);
	Projection3D(shader, numeroRepeticionesElementos, cubePositions, camera, numberOfElements);
	shader.Set("texture1", 0);
	shader.Set("texture2", 1);

	glDrawElements(GL_TRIANGLES, numberOfElements, GL_UNSIGNED_INT, 0);
}

uint32_t createTexture(const char* path, bool flip) {
	uint32_t texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
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

	string pathFinalImagen1String = utils.GetFinalPath(pathProyecto, "Textures/texture1.jpg");
	const char* pathFinalImagen1 = pathFinalImagen1String.c_str();

	string pathFinalImagen2String = utils.GetFinalPath(pathProyecto, "Textures/texture2.jpg");
	const char* pathFinalImagen2 = pathFinalImagen2String.c_str();

	Shader shader = Shader(vertexpath, fragmentPath1);
	uint32_t VBOFigura, EBO;


	long sizeOfIndices, sizeOfVertices;


	sizeOfIndices = numeroIndicesCubo * sizeof(float);
	sizeOfVertices = numeroElementosVerticesCubo * sizeof(float);



	vec3 origen = vec3(0.0f);
	int radio = 1.0f;


	//Cara delantera
	vec3 CaraDelantera[] = {
		vec3(-radio, -radio, radio), //Abajo izq	    1
		vec3(radio, -radio, radio), //Abajo Derecha  2 
		vec3(radio, radio, radio), //Arriba Dere	3
		vec3(-radio, radio, radio) //Arriba Izq		4

	};

	//Cara trasera
	vec3 CaraTrasera[] = {
		vec3(-radio, -radio, -radio), //Abajo izq		8
		vec3(-radio, radio, -radio), //Abajo Derecha	5
		vec3(radio, radio, -radio), //Arriba Dere		6
		vec3(radio, -radio, -radio) //Arriba Izq		7
	};

	//cara superior
	vec3 CaraSuperior[] = {
		vec3(-radio, radio, radio), //Abajo izq		4
		vec3(radio, radio, radio), //Abajo Derecha	3
		vec3(radio, radio, -radio), //Arriba Dere		6
		vec3(-radio, radio, -radio) //Arriba Izq		7
	};

	//cara Inferior
	vec3 CaraInferior[] = {
		vec3(-radio, -radio, radio), //Abajo izq		1
		vec3(-radio, -radio, -radio), //Abajo Derecha	2
		vec3(radio, -radio, -radio), //Arriba Dere		5
		vec3(radio, -radio, radio) //Arriba Izq		8
	};

	vec3 CaraDerecha[] = {
		vec3(radio, -radio, radio), //Abajo izq		1
		vec3(radio, -radio, -radio), //Abajo Derecha	2
		vec3(radio, radio, -radio), //Arriba Dere		5
		vec3(radio, radio, radio) //Arriba Izq		8
	};


	vec3 CaraIzquierda[] = {
		vec3(-radio, -radio, radio), //Abajo izq		1
		vec3(-radio, radio, radio), //Abajo Derecha	2
		vec3(-radio, radio, -radio), //Arriba Dere		5
		vec3(-radio, -radio, -radio) //Arriba Izq		8
	};



	float verticesCubo2[]{
		// Position					
			CaraDelantera[0].x, CaraDelantera[0].y, CaraDelantera[0].z,	0.0f, 0.0f,	//Front	
			CaraDelantera[1].x, CaraDelantera[1].y, CaraDelantera[1].z,	1.0f, 0.0f,
			CaraDelantera[2].x, CaraDelantera[2].y, CaraDelantera[2].z,	1.0f, 1.0f,
			CaraDelantera[3].x, CaraDelantera[3].y, CaraDelantera[3].z,	0.0f, 1.0f,

			CaraDerecha[0].x, CaraDerecha[0].y, CaraDerecha[0].z,	0.0f, 0.0f,	 //Right
			CaraDerecha[1].x, CaraDerecha[1].y, CaraDerecha[1].z,	1.0f, 0.0f,
			CaraDerecha[2].x, CaraDerecha[2].y, CaraDerecha[2].z,	1.0f, 1.0f,
			CaraDerecha[3].x, CaraDerecha[3].y, CaraDerecha[3].z,	0.0f, 1.0f,

			CaraTrasera[0].x, CaraTrasera[0].y, CaraTrasera[0].z,	0.0f, 0.0f,	 //Back
			CaraTrasera[1].x, CaraTrasera[1].y, CaraTrasera[1].z,	1.0f, 0.0f,
			CaraTrasera[2].x, CaraTrasera[2].y, CaraTrasera[2].z,	1.0f, 1.0f,
			CaraTrasera[3].x, CaraTrasera[3].y, CaraTrasera[3].z,	0.0f, 1.0f,

			CaraIzquierda[0].x, CaraIzquierda[0].y, CaraIzquierda[0].z,	0.0f, 0.0f,	 //Left
			CaraIzquierda[1].x, CaraIzquierda[1].y, CaraIzquierda[1].z,	1.0f, 0.0f,
			CaraIzquierda[2].x, CaraIzquierda[2].y, CaraIzquierda[2].z,	1.0f, 1.0f,
			CaraIzquierda[3].x, CaraIzquierda[3].y, CaraIzquierda[3].z,	0.0f, 1.0f,

			CaraInferior[0].x, CaraInferior[0].y, CaraInferior[0].z,	0.0f, 0.0f,	 //Bottom
			CaraInferior[1].x, CaraInferior[1].y, CaraInferior[1].z,	1.0f, 0.0f,
			CaraInferior[2].x, CaraInferior[2].y, CaraInferior[2].z,	1.0f, 1.0f,
			CaraInferior[3].x, CaraInferior[3].y, CaraInferior[3].z,	0.0f, 1.0f,

			CaraSuperior[0].x, CaraSuperior[0].y, CaraSuperior[0].z,	0.0f, 0.0f, //Top
			CaraSuperior[1].x, CaraSuperior[1].y, CaraSuperior[1].z,	1.0f, 0.0f,
			CaraSuperior[2].x, CaraSuperior[2].y, CaraSuperior[2].z,	1.0f, 1.0f,
			CaraSuperior[3].x, CaraSuperior[3].y, CaraSuperior[3].z,	0.0f, 1.0f
	};


	//float verticesQuad = cuadrado.GetVertexs();
	Buffer buffer = Buffer(sizeOfIndices, sizeOfVertices);

	uint32_t VAO = createVertexData(verticesCubo2, sizeOfIndices, indicesCubo, sizeOfVertices);


	uint32_t texture1 = createTexture(pathFinalImagen1, true);
	uint32_t texture2 = createTexture(pathFinalImagen2, true);


	//Bucle inicial donde se realiza toda la accion del motor
	while (!glfwWindowShouldClose(window.GetWindow())) {
		float currentFrame = glfwGetTime();
		float deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		HandlerInput(window.GetWindow(), deltaTime);
		window.HandlerInput();

		Render(VAO, shader, 36, texture1, texture2, 10, cubePositions, camera);

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

