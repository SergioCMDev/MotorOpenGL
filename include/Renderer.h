#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include "Shader.h"

class Renderer
{
public:
	void Render(uint32_t VAO, const Shader& shader, const uint32_t numberOfElements);
	void Render(uint32_t VAO, const Shader& shader);
	void Render(GLfloat R, GLfloat G, GLfloat B, GLfloat A);
	void Render();
	void Render(uint32_t VAO, uint32_t program);
	void CambiarColorUniform(Shader& shader, char* uniformName);
	void CambiarPosicionUniform(Shader& shader, char* uniformName);
};