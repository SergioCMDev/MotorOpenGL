#version 330 core

layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aNormal;
layout (location=2) in vec2 aTextCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMat;

out vec3 normal;
out vec3 fragPos;
out vec2 texCoord;





void main() {
	texCoord = aTextCoord;
	gl_Position = vec4(aPos,1.0);
}

