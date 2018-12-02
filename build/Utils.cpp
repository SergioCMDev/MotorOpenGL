#include "Utils.h"



Utils::Utils()
{
}


Utils::~Utils()
{
}

const char * Utils::GetPath(const char * pathOrigen, const char * pathSecundario)
{
	const char* path = pathOrigen;
	char* result = (char*)calloc(strlen(path) + strlen("Shaders/vertex.vs") + 1, sizeof(char));
	strcpy(result, path); // copy string one into the result.

	const char* vertexpath = strcat(result, "Shaders/vertex.vs");
}
