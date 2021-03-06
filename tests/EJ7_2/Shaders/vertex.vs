#version 330 core

layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aNormal;

out vec3 fragPos;
out vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMat;

out vec2 texCoords;

void main() {
	normal = normalMat * aNormal;
	fragPos = vec3(model * vec4(aPos, 1.0));
	 gl_Position = projection * view * model * vec4(aPos.x, aPos.y,aPos.z,1.0);
}

