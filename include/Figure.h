#ifndef FIGURE_H
#define FIGURE_H
#include<iostream>
#include <string>
enum Figuras {
	Triangulo,
	Cuadrado,
	Cubo
};
class Figure {


public:
	Figure();
	Figure(Figuras figura);
	float* GetVertexs();
	uint32_t* GetIndexs();
	uint32_t GetNumberOfElementsIndexs();
	uint32_t GetNumberOfElementsVertexs();
private:
	void InitVertices(Figuras figura);
	float* _vertex;
	uint32_t* _indexes;
	uint32_t _elementsIndexes;
	uint32_t _elementsVertexs;
};
#endif