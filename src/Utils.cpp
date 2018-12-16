#include "Utils.h"

string Utils::GetFinalPath(const string pathInicial, const string  variablePath)
{  
	string finalPath = pathInicial + variablePath;
	cout << finalPath.c_str() << endl;
	return finalPath;
}

