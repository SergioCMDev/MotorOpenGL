#ifndef RENDERER_H
#define RENDERER_H
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include "Shader.h"
#include "Camera.h"

class Renderer
{
public:
	void Render();
	void Render(uint32_t VAO, const Shader& shader);
	void Render(uint32_t VAO, uint32_t program);
	void Render(GLfloat R, GLfloat G, GLfloat B, GLfloat A);
	void Render(uint32_t VAO, const Shader& shader, const uint32_t numberOfElements);
	void RenderConMovimiento(uint32_t VAO, const Shader& shader, const uint32_t numberOfElements, bool movimiento, Camera camera);
	void Render(uint32_t VAO, const Shader& shader, const uint32_t numberOfElements, bool limpiarPantalla);
	void Render(uint32_t VAO, const Shader& shader, const uint32_t numberOfElements, uint32_t texture);
	void Render(uint32_t VAO, const Shader& shader, const uint32_t numberOfElements, uint32_t texture1, uint32_t texture2);
	void Render(uint32_t VAO, const Shader& shader, const uint32_t numberOfElements, uint32_t texture1, uint32_t texture2, bool movimiento);
	void Render(uint32_t VAO, const Shader& shader, const uint32_t numberOfElements, uint32_t texture1, uint32_t texture2, bool movimiento, uint32_t numeroRepeticionesElementos, glm::vec3 *cubePositions, Camera camera);
	void Render(uint32_t VAO, const Shader& shader, const uint32_t numberOfElements, uint32_t texture1, uint32_t texture2, bool movimiento, uint32_t numeroRepeticionesElementos, glm::vec3 *cubePositions);
	void ChangePosicionUniformRandom(Shader& shader, char* uniformName);
	void ChangePosicionUniform(Shader& shader, char* uniformName, float posX, float posY, float posZ);
	void CambiarColorUniformRandom(Shader& shader, char* uniformName);
	void CambiarColorUniform(Shader& shader, char* uniformName, float colorValue1, float colorValue2, float colorValue3);
	void ChangeInterpolationUniform(Shader& shader, char* uniformName, float value);
private:
	void Projection3D(const Shader & shader, bool movimiento);
	void Projection3D(const Shader & shader, bool movimiento, Camera camera);
	void Projection3D(const Shader & shader, bool movimiento, uint32_t numeroRepeticionesElementos, glm::vec3 *cubePositions);
	void Projection3D(const Shader & shader, bool movimiento, uint32_t numeroRepeticionesElementos, glm::vec3 *cubePositions, Camera camera);
	void Renderer::DrawElements(uint32_t numberOfElements);
	uint32_t _numberOfElementsToDraw;
};
#endif