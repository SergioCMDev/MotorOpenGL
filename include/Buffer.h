#pragma once
#ifndef BUFFER_H     
#define  BUFFER_H

#include<cstdint>
#include<stdio.h>
#include "Shader.h"

class Buffer {

public:
	Buffer();
	Buffer(long sizeOfIndices, long sizeOfVertices);
	uint32_t GetElementsPerLine();
	uint32_t GetElementsToDraw();

	uint32_t CreateVAO(uint32_t *VBO, uint32_t *EBO, uint32_t indices[],
		float vertices[], Shader* shader);

	uint32_t Buffer::CreateVAO(uint32_t *VBO, uint32_t *EBO, uint32_t indices[], uint32_t sizeOfIndices,
		float vertices[], uint32_t sizeOfVertices, Shader* shader);
	 void Buffer::SetStatusVerticesColor(bool cond);
	 void Buffer::SetStatusVerticesTextura(bool cond);
	 void Buffer::SetStatusVerticesNormal(bool cond);
	 void Buffer::SetSizeVerticesTextura(uint32_t size);
	 void Buffer::SetSizeVerticesColor(uint32_t size);
	 void Buffer::SetSizeVerticesNormal(uint32_t size);
private:
	uint32_t *VBO,  *EBO, _sizeOfVertices, _sizeOfIndices, _numberOfLines, _numberOfElementsPerLine, _numberOfElements, _elementsToDraw;
	bool _verticesColor = false;
	bool _verticesTextura = false;
	bool _verticesNormal = false;
	uint32_t _sizeVerticesTextura = 2;
	uint32_t _sizeVerticesColor = 3;
	uint32_t _sizeVerticesNormal = 3;
	
};
#endif