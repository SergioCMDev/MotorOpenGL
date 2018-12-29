#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<iostream>
#include<cstdint>
#include<stdio.h>

#include <Shader.h>
//#include <Renderer.h>
#include <Utils.h>
#include <Window.h>
#include <Buffer.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Utils utils;

using namespace std;
Window window;

const int widht = 800, height = 600;
const char* pathProyecto = "../tests/AG04/";

float quad_vertices_suelo[] = {
	// positions		// texture coords
-0.5f, -0.5f, 0.0f,			0.0f, 0.0f, // bottom left
0.5f, -0.5f, 0.0f,			1.0f, 0.0f, // bottom right
0.5f, 0.5f, 0.0f,			1.0f, 1.0f, // top right
-0.5f, 0.5f, 0.0f,			0.0f, 1.0f // top left
};



uint32_t quad_indices[] = {
	0, 1, 2, 0, 2, 3 //Front
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

void Projection3D(const Shader & shader, bool movimiento = false)
{

	glm::mat4 view = glm::mat4(1.0f);
	//alejamos el mundo 3D
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));


	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 10.0f);

	glm::mat4 model = glm::mat4(1.0f);
	float angle;
	if (movimiento) {
		angle = 10.0f + (cos(glfwGetTime()) + (sin(glfwGetTime())));
		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(0.5f, 1.0f, 0.0f));
	}

	//glm::mat4 transform = glm::mat4(1.0f);
	//transform = glm::rotate(transform, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	//transform = glm::scale(transform, glm::vec3(0.5f, 0.5f, 0.5f));

	shader.Set("view", view);
	shader.Set("model", model);
	shader.Set("projection", projection);
}

void Render(uint32_t VAO, const Shader& shader, const uint32_t numberOfElements, uint32_t texture1, uint32_t texture2) {
	//Renderizamos la pantalla con un color basandonos en el esquema RGBA(transparencia)
	//Si lo quitamos, no borra nunca la pantalla
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shader.Use();
	glActiveTexture(GL_TEXTURE0);	glBindTexture(GL_TEXTURE_2D, texture1);	glActiveTexture(GL_TEXTURE1);	glBindTexture(GL_TEXTURE_2D, texture2);
	//Bindeamos VAO
	glBindVertexArray(VAO);

	Projection3D(shader, true);
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

	string pathFinalImagen1String = utils.GetFinalPath(pathProyecto, "Textures/texture1.jpg");
	const char* pathFinalImagen1 = pathFinalImagen1String.c_str();

	string pathFinalImagen2String = utils.GetFinalPath(pathProyecto, "Textures/texture2.jpg");
	const char* pathFinalImagen2 = pathFinalImagen2String.c_str();



	Shader shader = Shader(vertexpath, fragmentPath1);
	int program = shader.GetIdProgram();
	uint32_t VBOTriangulo1, EBO;




	Buffer buffer = Buffer(sizeof(quad_indices), sizeof(quad_vertices_suelo));
	buffer.SetStatusVerticesColor(false);
	buffer.SetStatusVerticesTextura(true);
	uint32_t numberOfElementsToDraw = buffer.GetElementsToDraw();

	uint32_t VAO = createVertexData(quad_vertices_suelo, 24, quad_indices, 6);

	uint32_t texture1 = createTexture(pathFinalImagen1,true);

	uint32_t texture2 = createTexture(pathFinalImagen2,true);


	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//Bucle inicial donde se realiza toda la accion del motor
	while (!glfwWindowShouldClose(window.GetWindow())) {
		window.HandlerInput();

		Render(VAO, shader, numberOfElementsToDraw, texture1, texture2);

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

