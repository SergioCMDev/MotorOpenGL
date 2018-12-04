#include "Renderer.h"
#include "Shader.h"
#include<glad/glad.h>
#include<GLFW/glfw3.h>


void Renderer::Render(uint32_t VAO, const Shader& shader, const uint32_t numberOfElements) {
	//Renderizamos la pantalla con un color basandonos en el esquema RGBA(transparencia)
	//Si lo quitamos, no borra nunca la pantalla
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shader.Use();
	glBindVertexArray(VAO);
	//Bindeamos VAO

	glm::mat4 view = glm::mat4(1.0f);
	//alejamos el mundo 3
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 10.0f);

	glm::mat4 model = glm::mat4(1.0f);
	float angle = 10.0f + (cos(glfwGetTime()) +  (sin(glfwGetTime())));
	model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(0.5f, 1.0f, 0.0f));

	//glm::mat4 transform = glm::mat4(1.0f);
	//transform = glm::rotate(transform, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	//transform = glm::scale(transform, glm::vec3(0.5f, 0.5f, 0.5f));
	
	shader.Set("view", view);
	shader.Set("model", model);
	shader.Set("projection", projection);

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
	float posValue1 = sin(timeValue);
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
	float colorValue = sin(timeValue);
	int idProgram = shader.GetIdProgram();
	//si es -1 es error
	int vertexColorLocation = glGetUniformLocation(idProgram, uniformName);
	if (vertexColorLocation < 0) {
		cout << "Error al cargar Uniform " << uniformName << endl;
	}

	glUniform3f(vertexColorLocation, colorValue, colorValue2, colorValue3);
}
//
//glGetUniformfv Renderer::GetValorUniform(Shader& shader, char* uniformName) {
//	int idProgram = shader.GetIdProgram();
//	//si es -1 es error
//	int uniformPosition = glGetUniformLocation(idProgram, uniformName);
//	if (uniformPosition < 0) {
//		cout << "Error al cargar Uniform " << uniformName << endl;
//	}
//	/*gl.getUniform(program, loc);
//	return glGetUniform(idProgram, uniformPosition,0);*/
//}

void Renderer::CambiarColorUniform(Shader& shader, char* uniformName, float colorValue1, float colorValue2, float colorValue3) {
	float timeValue = glfwGetTime();
	//float colorValue = sin(timeValue);
	int idProgram = shader.GetIdProgram();
	//si es -1 es error
	int vertexColorLocation = glGetUniformLocation(idProgram, uniformName);
	if (vertexColorLocation < 0) {
		cout << "Error al cargar Uniform " << uniformName << endl;
	}

	glUniform3f(vertexColorLocation, colorValue1, colorValue2, colorValue3);
}
