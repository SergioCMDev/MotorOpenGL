#include "Figure.h"

Figure::Figure()
{
}

Figure::Figure(Figuras figura)
{
	InitVertices(figura,true);
}

Figure::Figure(Figuras figura, bool texture)
{
	InitVertices(figura, texture);
}

float* Figure::GetVertexs()
{
	return _vertex;
}

uint32_t* Figure::GetIndexs()
{
	return _indexes;
}

uint32_t Figure::GetNumberOfElementsIndexs() {
	return  _elementsIndexes;
}
uint32_t Figure::GetNumberOfElementsVertexs() {
	return _elementsVertexs;;

}

void Figure::InitVertices(Figuras figura, bool texture = false)
{
	switch (figura)
	{
	case Figuras::Cuadrado:
		_elementsVertexs = 20;
		if (texture) {
			_vertex = new float[_elementsVertexs] {
				// Position					// UVs
				-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,	//Front	
					0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
					0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
					-0.5f, 0.5f, 0.5f, 0.0f, 1.0f };
		}
		else {
			_vertex = new float[_elementsVertexs] {
				// Position					// UVs
				-0.5f, -0.5f, 0.5f,
					0.5f, -0.5f, 0.5f,
					0.5f, 0.5f, 0.5f,
					-0.5f, 0.5f, 0.5f };
		}
			_elementsIndexes = 6;

			_indexes = new uint32_t[_elementsIndexes]{
			0, 1, 2, 0, 2, 3 };

			break;
	case Figuras::Cubo:
		if (texture) {

		_elementsVertexs = 120;

		_vertex = new float[_elementsVertexs] {
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
		}
		else {
			_elementsVertexs = 72;

			_vertex = new float[_elementsVertexs] {
				// Position					// UVs
					-0.5f, -0.5f, 0.5f,//Front	
					0.5f, -0.5f, 0.5f,
					0.5f, 0.5f, 0.5f,
					-0.5f, 0.5f, 0.5f,

					0.5f, -0.5f, 0.5f,  //Right
					0.5f, -0.5f, -0.5f, 
					0.5f, 0.5f, -0.5f, 
					0.5f, 0.5f, 0.5f,

					-0.5f, -0.5f, -0.5f,  //Back
					-0.5f, 0.5f, -0.5f,
					0.5f, 0.5f, -0.5f,
					0.5f, -0.5f, -0.5f, 

					-0.5f, -0.5f, 0.5f,  //Left
					-0.5f, 0.5f, 0.5f, 
					-0.5f, 0.5f, -0.5f, 
					-0.5f, -0.5f, -0.5f, 

					-0.5f, -0.5f, 0.5f,  //Bottom
					-0.5f, -0.5f, -0.5f, 
					0.5f, -0.5f, -0.5f, 
					0.5f, -0.5f, 0.5f, 

					-0.5f, 0.5f, 0.5f,  //Top
					0.5f, 0.5f, 0.5f,
					0.5f, 0.5f, -0.5f, 
					-0.5f, 0.5f, -0.5f, 
			};
		}
		_elementsIndexes = 36;

		_indexes = new uint32_t[_elementsIndexes]{
			0, 1, 2, 0, 2, 3 //Front
			,4, 5, 6, 4, 6, 7 //Right
			,8, 9, 10, 8, 10, 11 //Back
			,12, 13, 14, 12, 14, 15 //Left
			,16, 17, 18, 16, 18, 19 //Bottom
			,20, 21, 22, 20, 22, 23 //Top
		};
		break;
	case Figuras::Triangulo:
		_elementsVertexs = 5;

		_vertex = new float[_elementsVertexs] {
			// Position					// UVs
			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f };	//Front	

		_elementsIndexes = 3;

		_indexes = new uint32_t[_elementsIndexes]{
		0, 1, 2 };

		break;
	default:
		break;
		}
	}
