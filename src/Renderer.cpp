#include "Renderer.h"
#include "Shader.h"
#include<glad/glad.h>
#include<GLFW/glfw3.h>


void Renderer::Render(uint32_t VAO, const Shader& shader, const uint32_t numberOfElements) {
	//Renderizamos la pantalla con un color basandonos en el esquema RGBA(transparencia)
	//Si lo quitamos, no borra nunca la pantalla
	glClear(GL_COLOR_BUFFER_BIT);
	shader.Use();
	//Bindeamos VAO
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, numberOfElements, GL_UNSIGNED_INT, 0);
}

void Renderer::Render(uint32_t VAO, const Shader& shader) {
	//Renderizamos la pantalla con un color basandonos en el esquema RGBA(transparencia)
	//glClear(GL_COLOR_BUFFER_BIT);
	shader.Use();
	//Bindeamos VAO
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
}

void Renderer::Render(GLfloat R, GLfloat G, GLfloat B, GLfloat A) {
	//Renderizamos la pantalla con un color basandonos en el esquema RGBA(transparencia)
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(R, G, B, A);
}

void Renderer::Render() {
	//Renderizamos la pantalla con un color basandonos en el esquema RGBA(transparencia)
	glClear(GL_COLOR_BUFFER_BIT);

	Renderer::Render(0.0f, 0.0f, 1.0f, 0.0f);
}

void Renderer::Render(uint32_t VAO, uint32_t program) {
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(program);

	//Bindeamos VAO
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

}

void Renderer::ChangePosicionUniform(Shader& shader, char* uniformName) {
	float timeValue = glfwGetTime();
	float posValue1 = sin(timeValue);	float posValue2 = cos(timeValue) ;
	int idProgram = shader.GetIdProgram();
	//si es -1 es error
	int vertexPositionLocation = glGetUniformLocation(idProgram, uniformName);
	if (vertexPositionLocation < 0) {
		cout << "Error al cargar Uniform " << uniformName << endl;
	}

	glUniform3f(vertexPositionLocation, posValue1 , posValue2, 0);
}


void Renderer::CambiarColorUniform(Shader& shader, char* uniformName) {
	float timeValue = glfwGetTime();
	float colorValue = sin(timeValue);	float colorValue2 = cos(timeValue);	float colorValue3 = sin(timeValue);
	int idProgram = shader.GetIdProgram();
	//si es -1 es error
	int vertexColorLocation = glGetUniformLocation(idProgram, uniformName);
	if (vertexColorLocation < 0) {
		cout << "Error al cargar Uniform " << uniformName << endl;
	}

	glUniform3f(vertexColorLocation, colorValue, colorValue2, colorValue3);
}

void Renderer::CambiarColorUniform(Shader& shader, char* uniformName, float colorValue1, float colorValue2, float colorValue3) {
	float timeValue = glfwGetTime();
	//float colorValue = sin(timeValue);	//float colorValue2 = cos(timeValue);	//float colorValue3 = sin(timeValue);
	int idProgram = shader.GetIdProgram();
	//si es -1 es error
	int vertexColorLocation = glGetUniformLocation(idProgram, uniformName);
	if (vertexColorLocation < 0) {
		cout << "Error al cargar Uniform " << uniformName << endl;
	}

	glUniform3f(vertexColorLocation, colorValue1, colorValue2, colorValue3);
}
