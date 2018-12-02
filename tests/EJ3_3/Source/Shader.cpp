#include "../Headers/Shader.h"
//#include "glad/glad.h"
//#include<fstream>
//#include<iostream>
//#include <string>
//#include <sstream>

using namespace std;
using namespace glm;

Shader::~Shader() {
	glDeleteProgram(_id);
}

void Shader::Use() const {
	glUseProgram(_id);
}

Shader::Shader(const char * vertexPath, const char * fragmentPath, const char * geometryPath)
{
	string sVertexCode, sFragmentCode, sGeometryCode;
	loadShader(vertexPath, &sVertexCode);
	loadShader(fragmentPath, &sFragmentCode);
	if (geometryPath) {
		loadShader(geometryPath, &sGeometryCode);
	}

	const char* VertexCode = sVertexCode.c_str();
	const char* FragmentCode = sFragmentCode.c_str();
	if (geometryPath) {
		const char* GeometryCode = sGeometryCode.c_str();
	}

	// &vertexShaderSource puntero a la cadena de arriba
	//NULL ya que solo vamos a usar una cadena
	// Subimos informacion a la gpu

	uint32_t vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &VertexCode, NULL);
	glCompileShader(vertex);
	CheckError(vertex, Type::Vertex);

	uint32_t fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &FragmentCode, NULL);
	glCompileShader(fragment);
	CheckError(fragment, Type::Fragment);

	uint32_t geometry;
	if (geometryPath) {
		const char* geometryCode = sGeometryCode.c_str();
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &geometryCode, NULL);
		glCompileShader(geometry);
		CheckError(geometry, Type::Geometry);
	}
	_id = glCreateProgram();

	glAttachShader(_id, vertex);
	glAttachShader(_id, fragment);
	if (geometryPath) {
		glAttachShader(_id, geometry);
	}
	glLinkProgram(_id);
	CheckError(_id, Type::Program);

	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if (geometryPath) {
		glDeleteShader(geometry);
	}
}

//Cargamos un fichero y lo metemos en la variable stream y devolvemos *code
void Shader::loadShader(const char * path, string * code)
{
	ifstream file;
	file.open(path, ios_base::in);
	if (file) {
		stringstream stream;
		stream << file.rdbuf();
		*code = stream.str();
	}
	else {
		cout << "Error loading stream " << path << endl;
	}
}

//comprobamos que se ha creado el shader correctamente
void Shader::CheckError(const uint32_t shader, const Type type)const {
	int success;
	char infoLog[512];
	switch (type) {
	case Type::Fragment:
	case Type::Vertex:
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			//ARREGLAR
			cout << "Error Compiling Shader " << (type == Type::Vertex ? "Vertex" : "Fragment") << endl << infoLog << endl;
		}
		break;
	case Type::Program:
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shader, 512, NULL, infoLog);
			cout << "Error Linking Program " << endl << infoLog << endl;
		}
		break;
	}
}


void Shader::Set(const char* name, const bool value) const {
	glUniform1i(glGetUniformLocation(_id, name), static_cast<int> (value));
}

void Shader::Set(const char* name, const int value) const {
	glUniform1i(glGetUniformLocation(_id, name), value);
}

void Shader::Set(const char* name, const float value) const {
	glUniform1f(glGetUniformLocation(_id, name), static_cast<float> (value));
}

void Shader::Set(const char* name, const float value1, const float value2) const {
	glUniform2f(glGetUniformLocation(_id, name), value1, value2);
}

void Shader::Set(const char* name, const float value1, const float value2, const float value3) const {
	glUniform3f(glGetUniformLocation(_id, name), value1, value2, value3);
}

void Shader::Set(const char* name, const float value1, const float value2, const float value3, const float value4) const {
	glUniform4f(glGetUniformLocation(_id, name), value1, value2, value3, value4);
}


void Shader::Set(const char* name, const vec2& value) const {
	glUniform2fv(glGetUniformLocation(_id, name), 1, value_ptr(value));

}

void Shader::Set(const char* name, const vec3& value) const {
	glUniform3fv(glGetUniformLocation(_id, name), 1, value_ptr(value));

}
void Shader::Set(const char* name, const vec4& value) const {
	glUniform4fv(glGetUniformLocation(_id, name), 1, value_ptr(value));

}

void Shader::Set(const char* name, const mat2& value) const {
	glUniformMatrix2fv(glGetUniformLocation(_id, name), 1, GL_FALSE, value_ptr(value));

}
void Shader::Set(const char* name, const mat3& value) const {
	glUniformMatrix3fv(glGetUniformLocation(_id, name), 1, GL_FALSE, value_ptr(value));

}
void Shader::Set(const char* name, const mat4& value) const {
	glUniformMatrix4fv(glGetUniformLocation(_id, name), 1, GL_FALSE, value_ptr(value));

}