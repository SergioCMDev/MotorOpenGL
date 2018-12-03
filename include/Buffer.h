//#include<glad/glad.h>
//#include<GLFW/glfw3.h>

//#include<iostream>
#include<cstdint>
#include<stdio.h>
#include "Shader.h"

class Buffer {

public:
	Buffer();
	Buffer(long sizeOfIndices, long sizeOfVertices);
	uint32_t GetElementsPerLine();


	uint32_t CreateVAO(uint32_t *VBO, uint32_t *EBO, uint32_t indices[], uint32_t sizeOfIndices,
		float vertices[], uint32_t sizeOfVertices, Shader* shader, uint32_t *numberElementsPerLine);


	uint32_t CreateVAO(uint32_t *VBO, uint32_t *EBO, uint32_t indices[],
		float vertices[], Shader* shader);

	uint32_t Buffer::CreateVAO(uint32_t *VBO, uint32_t *EBO, uint32_t indices[], uint32_t sizeOfIndices,
		float vertices[], uint32_t sizeOfVertices, Shader* shader);

private:
	uint32_t *VBO,  *EBO, _sizeOfVertices, _sizeOfIndices, _numberOfLines, _numberOfElementsPerLine, _numberOfElements;

};
