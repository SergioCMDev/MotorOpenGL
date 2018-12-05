#include "Figure.h"

Figure::Figure()
{
}

Figure::Figure(Figuras figura)
{
	InitVertices(figura);
}

float* Figure::GetVertexs()
{
	return _vertex;
}

uint32_t* Figure::GetIndexs()
{
	return _indexes;
}

uint32_t Figure::GetSizeIndexs() {
	return  _sizeIndexes;
}
uint32_t Figure::GetSizeVertexs() {
	return _sizeVertexs;;

}

void Figure::InitVertices(Figuras figura)
{
	switch (figura)
	{
	case Figuras::Cuadrado:
		_sizeVertexs = 20;
		_vertex = new float[_sizeVertexs] {
			// Position					// UVs
			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,	//Front	
				0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
				0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
				-0.5f, 0.5f, 0.5f, 0.0f, 1.0f };
		_sizeIndexes = 6;
		_indexes = new uint32_t[_sizeIndexes]{
		0, 1, 2, 0, 2, 3 };

		break;
	case Figuras::Cubo:
		_sizeVertexs = 120;

		_vertex = new float[_sizeVertexs] {
			// Position					// UVs
			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,	//Front	
				0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
				0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
				-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,

				0.5f, -0.5f, 0.5f, 0.0f, 0.0f, //Right
				0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
				0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
				0.5f, 0.5f, 0.5f, 0.0f, 1.0f,

				-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, //Back
				-0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
				0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
				0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

				-0.5f, -0.5f, 0.5f, 1.0f, 0.0f, //Left
				-0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
				-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
				-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

				-0.5f, -0.5f, 0.5f, 0.0f, 1.0f, //Bottom
				-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
				0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
				0.5f, -0.5f, 0.5f, 1.0f, 1.0f,

				-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, //Top
				0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
				0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
				-0.5f, 0.5f, -0.5f, 0.0f, 1.0f
		};
		_sizeIndexes = 36;

		_indexes = new uint32_t[_sizeIndexes]{
			0, 1, 2, 0, 2, 3 //Front
			,4, 5, 6, 4, 6, 7 //Right
			,8, 9, 10, 8, 10, 11 //Back
			,12, 13, 14, 12, 14, 15 //Left
			,16, 17, 18, 16, 18, 19 //Bottom
			,20, 21, 22, 20, 22, 23 //Top
		};
		break;
	case Figuras::Triangulo:
		_sizeVertexs = 5;

		_vertex = new float[_sizeVertexs] {
			// Position					// UVs
			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f };	//Front	

		_sizeIndexes = 3;

		_indexes = new uint32_t[_sizeIndexes]{
		0, 1, 2 };

		break;
	default:
		break;
	}
}
