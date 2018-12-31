#pragma once
#ifndef SHADER_H__
#define SHADER_H_ 
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "glad/glad.h"
#include<fstream>
#include<iostream>
#include <string>
#include <sstream>
#include "glm/gtc/type_ptr.hpp"
using namespace glm;
using namespace std;
class Shader
{
	enum class Type {
		Vertex = 0,
		Fragment = 1,
		Geometry = 2,
		Program = 3,
	};

public:
	Shader();
	//Shader() = delete;
	Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
	uint32_t CreateShader(const char *vertexPath);
	uint32_t CreateFragment(const char *fragmentPath);
	uint32_t CreateGeometry(const char *geometryPath);
	void CreateProgram(uint32_t vertex,  uint32_t fragment,  uint32_t geometry);
	uint32_t GetIdProgram();
	~Shader();
	void Use() const;
	void Set(const char* name, const bool value) const;
	void Set(const char* name, const int value) const;
	void Set(const char* name, const float value) const;
	void Set(const char* name, const float value1, const float value2) const;
	void Set(const char* name, const float value1, const float value2, const float value3) const;
	void Set(const char* name, const float value1, const float value2, const float value3, const float value4) const;

	void Set(const char* name, const vec2& value) const;
	void Set(const char* name, const vec3& value) const;
	void Set(const char* name, const vec4& value) const;

	void Set(const char* name, const mat2& value) const;
	void Set(const char* name, const mat3& value) const;
	void Set(const char* name, const mat4& value) const;

private:
	void CheckError(const uint32_t shader, const Type type, const char* path) const;
	void loadShader(const char* path, string* code);
	uint32_t _id;
};
#endif 

