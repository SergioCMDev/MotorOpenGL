#include "Shader.h"

using namespace std;
using namespace glm;

const int numberOfElementsLoaded = 1;
Shader::~Shader() {
	glDeleteProgram(_id);
}

Shader::Shader() {
	//glDeleteProgram(_id);
}

void Shader::Use() const {
	glUseProgram(_id);
}


uint32_t Shader::GetIdProgram() {
	return _id;
}

uint32_t Shader::CreateShader(const char *vertexPath) {
	string sVertexCode;
	loadShader(vertexPath, &sVertexCode);
	const char* VertexCode = sVertexCode.c_str();
	uint32_t vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, numberOfElementsLoaded, &VertexCode, NULL);
	glCompileShader(vertex);
	CheckError(vertex, Shader::Type::Vertex, vertexPath);
	return vertex;
}


uint32_t Shader::CreateFragment(const char *fragmentPath) {
	string sFragmentCode;
	loadShader(fragmentPath, &sFragmentCode);
	const char* FragmentCode = sFragmentCode.c_str();
	uint32_t fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, numberOfElementsLoaded, &FragmentCode, NULL);
	glCompileShader(fragment);
	CheckError(fragment, Shader::Type::Fragment, fragmentPath);
	return fragment;
}

uint32_t Shader::CreateGeometry(const char *geometryPath) {
	string sGeometryCode;
	loadShader(geometryPath, &sGeometryCode);
	const char* GeometryCode = sGeometryCode.c_str();
	uint32_t geometry = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(geometry, numberOfElementsLoaded, &GeometryCode, NULL);
	glCompileShader(geometry);
	CheckError(geometry, Type::Geometry, geometryPath);
	return geometry;
}

void Shader::CreateProgram(uint32_t vertex, uint32_t fragment, uint32_t geometry) {
	_id = glCreateProgram();
	glAttachShader(_id, vertex);
	glAttachShader(_id, fragment);
	if (geometry) {
		glAttachShader(_id, geometry);
	}
	glLinkProgram(_id);
	CheckError(_id, Type::Program, 0);

}



Shader::Shader(const char * vertexPath, const char * fragmentPath, const char * geometryPath)
{
	uint32_t vertex = CreateShader(vertexPath);
	uint32_t fragment = CreateFragment(fragmentPath);
	uint32_t geometry = 0;
	if (geometryPath) {
		geometry = CreateGeometry(geometryPath);
	}

	CreateProgram(vertex, fragment, geometry);

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
void Shader::CheckError(const uint32_t shader, const Type type, const char * path)const {
	int success;
	char infoLog[512];
	switch (type) {
	case Type::Fragment:
	case Type::Vertex:
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			//ARREGLAR
			cout << "Error Compiling Shader " << (type == Type::Vertex ? "Vertex" : "Fragment") << path <<endl << infoLog << endl;
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