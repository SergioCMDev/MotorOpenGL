#include "Utils.h"
#include<iostream>

string Utils::GetFinalPath(const string pathInicial, const string  variablePath)
{  
	//cout<< variablePath.c_str() <<endl;
	//int sizePathInicial = pathInicial.size();
	//int sizeVariablePath = variablePath.size();
	string finalPath = pathInicial + variablePath;
	cout << finalPath.c_str() << endl;

	//string result = char[sizePathInicial + sizeVariablePath + 1];
	//strcpy(result, pathInicial.c_str()); // copy string one into the result.
	//char* finalPath = strcat(result, variablePath.c_str());
	return finalPath;

}

