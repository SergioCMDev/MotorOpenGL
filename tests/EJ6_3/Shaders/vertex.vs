#version 330 core

layout (location=0) in vec3 aPos;
//layout (location=1) in vec2 aTexCoord;
layout (location=1) in vec3 aNormal;

out vec3 fragPos;
out vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMat;



void main() {
	normal = normalMat * aNormal;
	fragPos = vec3((view * model) * vec4(aPos, 1.0));
	 gl_Position = projection * view * model * vec4(aPos.x, aPos.y,aPos.z,1.0);
}

