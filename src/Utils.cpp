#include "Utils.h"

char * Utils::GetFinalPath(const char * pathInicial, const char * variablePath)
{
	char* result = (char*)calloc(strlen(pathInicial) + strlen(variablePath) + 1, sizeof(char));
	strcpy(result, pathInicial); // copy string one into the result.
	char* finalPath = strcat(result, variablePath);
	return finalPath;

}
