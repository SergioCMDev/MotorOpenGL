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
#include <Texture.h>

Utils utils;

using namespace std;
Window window;

const int widht = 800, height = 600;
const char* pathProyecto = "../tests/AG04/";

float verticesCuadrado[] = {
	// positions		// texture coords
-0.5f, -0.5f, 0.0f,			0.0f, 0.0f, // bottom left
0.5f, -0.5f, 0.0f,			1.0f, 0.0f, // bottom right
0.5f, 0.5f, 0.0f,			1.0f, 1.0f, // top right
-0.5f, 0.5f, 0.0f,			0.0f, 1.0f // top left
};



uint32_t indicesCuadrado[] = {
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

int main(int argc, char* argv[]) {
	if (!Inicializacion()) {
		return -1;
	}
	const char* vertexpath = utils.GetFinalPath(pathProyecto, "Shaders/vertex.vs");
	const char* fragmentPath1 = utils.GetFinalPath(pathProyecto, "Shaders/fragment.fs");

	Shader shader = Shader(vertexpath, fragmentPath1);
	int program = shader.GetIdProgram();
	uint32_t VBOTriangulo1, EBO;




	Buffer buffer = Buffer(sizeof(indicesCuadrado), sizeof(verticesCuadrado));
	buffer.SetStatusVerticesColor(false);
	buffer.SetStatusVerticesTextura(true);
	uint32_t numberOfElementsToDraw = buffer.GetElementsToDraw();

	uint32_t VAO = buffer.CreateVAO(&VBOTriangulo1, &EBO, indicesCuadrado, verticesCuadrado, &shader);

	char* pathFinalImagen1 = utils.GetFinalPath(pathProyecto, "Textures/texture1.jpg");
	char* pathFinalImagen2 = utils.GetFinalPath(pathProyecto, "Textures/texture2.jpg");

	Texture image1 = Texture(pathFinalImagen1, 1024, 1024, 1, 0,true);
	image1.LoadTexture();
	Texture image2 = Texture(pathFinalImagen2, 1024, 1024, 1, 0, true);
	image2.LoadTexture();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//Bucle inicial donde se realiza toda la accion del motor
	while (!glfwWindowShouldClose(window.GetWindow())) {
		window.HandlerInput();

		Render(VAO, shader, numberOfElementsToDraw, image1.GetTexture(), image2.GetTexture());

		glfwSwapBuffers(window.GetWindow());
		glfwPollEvents();
	}

	//Si se han linkado bien los shaders, los borramos ya que estan linkados
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBOTriangulo1);
	glDeleteBuffers(1, &EBO);
	image1.ReleaseTexture();
	image2.ReleaseTexture();

	glfwTerminate();
	return 0;
}
#pragma endregion

