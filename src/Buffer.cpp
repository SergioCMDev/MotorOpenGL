#include "Buffer.h"

static struct sizesGeometria{
	int Triangulo = 3;
	int Cuadrado = 6;
	int Cubo = 36;
};

uint32_t Buffer::GetElementsPerLine() {
	return _numberOfElementsPerLine;
}

uint32_t Buffer::GetElementsToDraw() {
	return _elementsToDraw;
}
Buffer::Buffer() {

}


Buffer::Buffer(long sizeOfIndices, long sizeOfVertices) {

	//uint32_t sizeOfIndices = sizeof(indicesQuad); //3 uint32_t * sizeofuint32_t(4) = 12
	sizesGeometria sizes;
	_sizeOfIndices = sizeOfIndices;
	_sizeOfVertices = sizeOfVertices;
	if (sizeOfIndices / sizeof(float) == sizes.Triangulo) {
		cout << "Triangulo" << endl;
		_numberOfLines = 3; //72 vertices / sizeoffloat(4) = 18
	//_elementsToDraw = sizeOfIndices / _numberOfLines;  //Cuadrado 24/4 = 6, Triangulo 12/

	}
	else if (sizeOfIndices / sizeof(float) == sizes.Cuadrado) {
		cout << "Cuadrado" << endl;
		_numberOfLines = 4; //72 vertices / sizeoffloat(4) = 18
	}
	else if (sizeOfIndices / sizeof(float) == sizes.Cubo) {
		cout << "Cubo" << endl;
		_numberOfLines = 24; //72 vertices / sizeoffloat(4) = 18
	}

	_elementsToDraw = sizeOfIndices / sizeof(float);
	_numberOfElements = sizeOfVertices / sizeof(float); //72 vertices / sizeoffloat(4) = 18
	_numberOfElementsPerLine = _numberOfElements / _numberOfLines; //72 vertices / sizeoffloat(4) = 18

}



uint32_t Buffer::CreateVAO(uint32_t *VBO, uint32_t *EBO, uint32_t indices[], uint32_t sizeOfIndices,
	float vertices[], uint32_t sizeOfVertices, Shader* shader, uint32_t *elementsPerLine) {
	uint32_t VAO;
	glGenVertexArrays(1, &VAO);
	//Generamos 2 buffer, elementos y objetos
	glGenBuffers(1, VBO);
	glGenBuffers(1, EBO);

	//Bindeamos el VAO
	glBindVertexArray(VAO);


	//Bindeamos buffer vertices
	glBindBuffer(GL_ARRAY_BUFFER, *VBO);
	//Subida de vertices al buffer
	glBufferData(GL_ARRAY_BUFFER, sizeOfVertices, vertices, GL_STATIC_DRAW);

	//Bindeo buffer indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeOfIndices, indices, GL_STATIC_DRAW);

	//vertices del triangulo 6 por que hay 6 elementos hasta el proximo inicio de linea
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, *elementsPerLine * sizeof(float), (void*)0);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glad_glEnableVertexAttribArray(0);

	//Vertices de color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, *elementsPerLine * sizeof(float), (void*)(3 * sizeof(float)));
	//////Lo habilitamos
	glad_glEnableVertexAttribArray(1);

	//desbindeamos buffer objetos
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Desbindeo
	glBindVertexArray(0);

	//desbindeamos buffer elements
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return VAO;
}

uint32_t Buffer::CreateVAO(uint32_t *VBO, uint32_t *EBO, uint32_t indices[], uint32_t sizeOfIndices,
	float vertices[], uint32_t sizeOfVertices, Shader* shader) {
	uint32_t VAO;
	uint32_t elementsPerLine = 6;
	glGenVertexArrays(1, &VAO);
	//Generamos 2 buffer, elementos y objetos
	glGenBuffers(1, VBO);
	glGenBuffers(1, EBO);

	//Bindeamos el VAO
	glBindVertexArray(VAO);


	//Bindeamos buffer vertices
	glBindBuffer(GL_ARRAY_BUFFER, *VBO);
	//Subida de vertices al buffer
	glBufferData(GL_ARRAY_BUFFER, sizeOfVertices, vertices, GL_STATIC_DRAW);

	//Bindeo buffer indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeOfIndices, indices, GL_STATIC_DRAW);

	//vertices del triangulo 6 por que hay 6 elementos hasta el proximo inicio de linea
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, elementsPerLine * sizeof(float), (void*)0);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glad_glEnableVertexAttribArray(0);

	//Vertices de color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, elementsPerLine * sizeof(float), (void*)(3 * sizeof(float)));
	//////Lo habilitamos
	glad_glEnableVertexAttribArray(1);

	//desbindeamos buffer objetos
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Desbindeo
	glBindVertexArray(0);

	//desbindeamos buffer elements
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return VAO;
}


uint32_t Buffer::CreateVAO(uint32_t *VBO, uint32_t *EBO, uint32_t indices[],
	float vertices[], Shader* shader) {
	uint32_t VAO;
	uint32_t elementsPerLine = _numberOfElementsPerLine;;
	glGenVertexArrays(1, &VAO);
	//Generamos 2 buffer, elementos y objetos
	glGenBuffers(1, VBO);
	glGenBuffers(1, EBO);

	//Bindeamos el VAO
	glBindVertexArray(VAO);


	//Bindeamos buffer vertices
	glBindBuffer(GL_ARRAY_BUFFER, *VBO);
	//Subida de vertices al buffer
	glBufferData(GL_ARRAY_BUFFER, _sizeOfVertices, vertices, GL_STATIC_DRAW);

	//Bindeo buffer indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _sizeOfIndices, indices, GL_STATIC_DRAW);

	//vertices del triangulo 6 por que hay 6 elementos hasta el proximo inicio de linea
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, _numberOfElementsPerLine * sizeof(float), (void*)0);
	glad_glEnableVertexAttribArray(0);

	//Vertices de color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, _numberOfElementsPerLine * sizeof(float), (void*)(3 * sizeof(float)));
	//////Lo habilitamos
	glad_glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, _numberOfElementsPerLine * sizeof(float),	(void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//desbindeamos buffer objetos
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Desbindeo
	glBindVertexArray(0);

	//desbindeamos buffer elements
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return VAO;
}