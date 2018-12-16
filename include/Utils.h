#ifndef UTILS_H
#define UTILS_H
#include<iostream>
#include "string.h"

#include<stdio.h>
using namespace std;

class Utils {

public:
	string GetFinalPath(const string pathInicial, const string pathFinal);
};
#endif