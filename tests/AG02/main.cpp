#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<iostream>
#include<cstdint>
#include<stdio.h>
using namespace std;

#pragma region Cabezeras

void Render(GLfloat R, GLfloat G, GLfloat B, GLfloat A);
void Render();
void HandlerInput(GLFWwindow* window);
void OnChangeFrameBufferSize(GLFWwindow* window, const int32_t width, const int32_t height);
#pragma endregion


#pragma region Metodos




uint32_t createvectexData(uint32_t *VBO, uint32_t *EBO) {
	float vertices[] = {0.5f, 0.5f, 0.0f,
						0.5f, -0.5f, 0.0f,
	}
}
void Render(GLfloat R, GLfloat G, GLfloat B, GLfloat A) {
	//Renderizamos la pantalla con un color basandonos en el esquema RGBA(transparencia)
	glClearColor(R, G, B, A);
	glClear(GL_COLOR_BUFFER_BIT);
}
void Render() {
	//Renderizamos la pantalla con un color basandonos en el esquema RGBA(transparencia)
	Render(0.0f, 0.0f, 1.0f, 0.0f);
}
void HandlerInput(GLFWwindow* window) {
	//Si pulsamos la tecla ESC cerramos la ventana
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}



void OnChangeFrameBufferSize(GLFWwindow* window, const int32_t width, const int32_t height) {
	//redimension de pantalla 
	//Cambio de clip scene a view scene
	glViewport(0, 0, width, height);
}

bool checkShader(uint32_t shader) {
	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		cout << "Error Compilin Shader" << endl << infoLog << endl;
		return false;
	}
	return true;

}

bool checkProgram(uint32_t program) {
	int success;
	char infoLog[512]program
		glad_glGetProgramInfoLog(shader, 512, NULL, infLog);
	if (!success) {

		cout << "Error Compilin Shader" << endl << infoLog << endl;
		return false;
	}
	return true;
}

uint32_t createProgram() {
	const char * vertexShaderSource = "#version 330 core\n"
		"layout (localition = 0) in vec3 aPos;\n"
		"void main(){\n"
		" gl_position = vec4(aPos.X, aPosY, aPos.z, 1.0f);\n"
		"}\0";
	const char *fragmentShaderSource = "@version 330 core\n"
		"out vect4 fragColor; \n"
		"void main() {\n"
		"FragColor = vec4(0.7, 0.2, 0.2, 1.0)\n"
		"}\0";

	uint32_t vertexShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	checkShader(vertexShader);
}

int main(int argc, char* argv[]) {
	if (!glfwInit()) {
		cout << "Error initializing GLFW" << endl;
		glfwTerminate();
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Window 1", NULL, NULL);
	if (!window) {
		cout << "Error creating window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Error initializing GLAD" << endl;
		return -1;
	}

	//cuando la ventana cambie de tamaño
	glfwSetFramebufferSizeCallback(window, OnChangeFrameBufferSize);


	uint32_t

		//Bucle inicial donde se realiza toda la accion del motor
		while (!glfwWindowShouldClose(window)) {
			HandlerInput(window);
			Render(1.0f, 0.0f, 0.0f, 1.0f);
			glfwSwapBuffers(window);
			glfwPollEvents();
		}

	//Crear metodo para esto
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glDeleteProgram(program);

	glfwTerminate();
	return 0;
}
#pragma endregion