//#include<glad/glad.h>
//#include<GLFW/glfw3.h>

//#include<iostream>
#include<cstdint>
#include<stdio.h>
#include "Shader.h"

class Buffer {

public:
	Buffer();

	uint32_t CreateVAO(uint32_t *VBO, uint32_t *EBO, uint32_t indices[], uint32_t sizeOfIndices,
		float vertices[], uint32_t sizeOfVertices, Shader* shader);
private:
	uint32_t *VBO,  *EBO;

};
