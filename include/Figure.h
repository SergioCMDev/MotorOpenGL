#include<iostream>
#include <string>
enum Figuras {
	Triangulo,
	Cuadrado,
	Cubo,
};
class Figure {


public:
	Figure();
	Figure(Figuras figura);
	float* GetVertexs();
	uint32_t* GetIndexs();
	uint32_t GetSizeIndexs();
	uint32_t GetSizeVertexs();
private:
	void InitVertices(Figuras figura);
	float* _vertex;
	uint32_t* _indexes;
	uint32_t _sizeIndexes;
	uint32_t _sizeVertexs;
};