#include "Renderer.h"
#include "Shader.h"
#include<glad/glad.h>
#include<GLFW/glfw3.h>


void Renderer::Render(uint32_t VAO, const Shader& shader, const uint32_t numberOfElements, uint32_t texture) {
	//Renderizamos la pantalla con un color basandonos en el esquema RGBA(transparencia)
	//Si lo quitamos, no borra nunca la pantalla
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	shader.Use();
	//Bindeamos VAO
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);	glBindVertexArray(VAO);

	Projection3D(shader, true);

	glDrawElements(GL_TRIANGLES, numberOfElements, GL_UNSIGNED_INT, 0);
}

void Renderer::Render(uint32_t VAO, const Shader& shader, const uint32_t numberOfElements, uint32_t texture1, uint32_t texture2) {
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

void Renderer::Render(uint32_t VAO, const Shader& shader, const uint32_t numberOfElements, uint32_t texture1, uint32_t texture2, bool movimiento) {
	//Renderizamos la pantalla con un color basandonos en el esquema RGBA(transparencia)
	//Si lo quitamos, no borra nunca la pantalla
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shader.Use();
	glActiveTexture(GL_TEXTURE0);	glBindTexture(GL_TEXTURE_2D, texture1);	glActiveTexture(GL_TEXTURE1);	glBindTexture(GL_TEXTURE_2D, texture2);
	//Bindeamos VAO
	glBindVertexArray(VAO);

	Projection3D(shader, movimiento);
	shader.Set("texture1", 0);
	shader.Set("texture2", 1);

	glDrawElements(GL_TRIANGLES, numberOfElements, GL_UNSIGNED_INT, 0);
}

void Renderer::Render(uint32_t VAO, const Shader& shader, const uint32_t numberOfElements, uint32_t texture1,
	uint32_t texture2, bool movimiento, uint32_t numeroRepeticionesElementos, glm::vec3 *cubePositions, Camera camera) {
	//Renderizamos la pantalla con un color basandonos en el esquema RGBA(transparencia)
	//Si lo quitamos, no borra nunca la pantalla
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	_numberOfElementsToDraw = numberOfElements;
	shader.Use();
	glActiveTexture(GL_TEXTURE0);	glBindTexture(GL_TEXTURE_2D, texture1);	glActiveTexture(GL_TEXTURE1);	glBindTexture(GL_TEXTURE_2D, texture2);
	//Bindeamos VAO
	glBindVertexArray(VAO);
	Projection3D(shader, movimiento, numeroRepeticionesElementos, cubePositions, camera);
	shader.Set("texture1", 0);
	shader.Set("texture2", 1);

	glDrawElements(GL_TRIANGLES, numberOfElements, GL_UNSIGNED_INT, 0);
}


void Renderer::Render(uint32_t VAO, const Shader& shader, const uint32_t numberOfElements, uint32_t texture1,
	uint32_t texture2, bool movimiento, uint32_t numeroRepeticionesElementos, glm::vec3 *cubePositions) {
	//Renderizamos la pantalla con un color basandonos en el esquema RGBA(transparencia)
	//Si lo quitamos, no borra nunca la pantalla
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	_numberOfElementsToDraw = numberOfElements;
	shader.Use();
	glActiveTexture(GL_TEXTURE0);	glBindTexture(GL_TEXTURE_2D, texture1);	glActiveTexture(GL_TEXTURE1);	glBindTexture(GL_TEXTURE_2D, texture2);
	//Bindeamos VAO
	glBindVertexArray(VAO);
	Projection3D(shader, movimiento, numeroRepeticionesElementos, cubePositions);
	shader.Set("texture1", 0);
	shader.Set("texture2", 1);

	glDrawElements(GL_TRIANGLES, numberOfElements, GL_UNSIGNED_INT, 0);
}

void Renderer::Render(uint32_t VAO, const Shader& shader, const uint32_t numberOfElements) {
	//Renderizamos la pantalla con un color basandonos en el esquema RGBA(transparencia)
	//Si lo quitamos, no borra nunca la pantalla

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader.Use();
	glBindVertexArray(VAO);
	//Bindeamos VAO

	Projection3D(shader, true);

	glDrawElements(GL_TRIANGLES, numberOfElements, GL_UNSIGNED_INT, 0);
}

void Renderer::RenderConMovimiento(uint32_t VAO, const Shader& shader, const uint32_t numberOfElements, bool movimiento, Camera camera) {
	//Renderizamos la pantalla con un color basandonos en el esquema RGBA(transparencia)
	//Si lo quitamos, no borra nunca la pantalla

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader.Use();
	glBindVertexArray(VAO);
	//Bindeamos VAO

	Projection3D(shader, movimiento, camera);

	glDrawElements(GL_TRIANGLES, numberOfElements, GL_UNSIGNED_INT, 0);
}

void Renderer::Render(uint32_t VAO, const Shader& shader, const uint32_t numberOfElements, bool limpiarPantalla) {
	//Renderizamos la pantalla con un color basandonos en el esquema RGBA(transparencia)
	//Si lo quitamos, no borra nunca la pantalla
	if (limpiarPantalla) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	shader.Use();
	glBindVertexArray(VAO);
	//Bindeamos VAO

	Projection3D(shader, true);

	glDrawElements(GL_TRIANGLES, numberOfElements, GL_UNSIGNED_INT, 0);
}

void Renderer::Projection3D(const Shader & shader, bool movimiento = false)
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

void Renderer::Projection3D(const Shader & shader, bool movimiento, Camera camera)
{


	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(camera.GetFOV()), 800.0f / 600.0f, 0.1f, 10.0f);

	glm::mat4 model = glm::mat4(1.0f);
	float angle;
	if (movimiento) {
		angle = 10.0f + (cos(glfwGetTime()) + (sin(glfwGetTime())));
		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(0.5f, 1.0f, 0.0f));
	}

	//glm::mat4 transform = glm::mat4(1.0f);
	//transform = glm::rotate(transform, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	//transform = glm::scale(transform, glm::vec3(0.5f, 0.5f, 0.5f));

	shader.Set("view", camera.GetViewMatrix());
	shader.Set("model", model);
	shader.Set("projection", projection);
}

void Renderer::Projection3D(const Shader & shader, bool movimiento, uint32_t numeroRepeticionesElemento, glm::vec3 *cubePositions)
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
			DrawElements(_numberOfElementsToDraw);
		}
	}
	shader.Set("view", view);
	shader.Set("projection", projection);
}

void Renderer::Projection3D(const Shader & shader, bool movimiento, uint32_t numeroRepeticionesElemento, glm::vec3 *cubePositions, Camera camera)
{


	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(camera.GetFOV()), 800.0f / 600.0f, 0.1f, 10.0f);

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
			DrawElements(_numberOfElementsToDraw);
		}
	}
	shader.Set("view", camera.GetViewMatrix());
	shader.Set("projection", projection);
}

void Renderer::DrawElements(uint32_t numberOfElements) {
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

void Renderer::ChangePosicionUniformRandom(Shader& shader, char* uniformName) {
	float timeValue = glfwGetTime();
	float posValue1 = sin(timeValue);	float posValue2 = cos(timeValue);
	Renderer::ChangePosicionUniform(shader, uniformName, posValue1, posValue2, 0);
}

void Renderer::ChangePosicionUniform(Shader& shader, char* uniformName, float posX, float posY, float posZ) {

	int idProgram = shader.GetIdProgram();
	//si es -1 es error
	int vertexPositionLocation = glGetUniformLocation(idProgram, uniformName);
	if (vertexPositionLocation < 0) {
		cout << "Error al cargar Uniform " << uniformName << endl;
	}

	glUniform3f(vertexPositionLocation, posX, posY, posZ);
}


void Renderer::ChangeInterpolationUniform(Shader& shader, char* uniformName, float value) {

	int idProgram = shader.GetIdProgram();
	//si es -1 es error
	int vertexPositionLocation = glGetUniformLocation(idProgram, uniformName);
	if (vertexPositionLocation < 0) {
		cout << "Error al cargar Uniform " << uniformName << endl;
	}

	glUniform1f(vertexPositionLocation, value);
}


void Renderer::CambiarColorUniformRandom(Shader& shader, char* uniformName) {
	float timeValue = glfwGetTime();
	float colorValue = sin(timeValue);	float colorValue2 = cos(timeValue);	float colorValue3 = sin(timeValue);	CambiarColorUniform(shader, uniformName, colorValue, colorValue2, colorValue3);
}

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

	int idProgram = shader.GetIdProgram();
	//si es -1 es error
	int vertexColorLocation = glGetUniformLocation(idProgram, uniformName);
	if (vertexColorLocation < 0) {
		cout << "Error al cargar Uniform " << uniformName << endl;
	}

	glUniform3f(vertexColorLocation, colorValue1, colorValue2, colorValue3);
}

