#version 330 core
layout (location=1) in vec3 aColor;
layout (location=0) in vec3 aPos;

out vec3 myColor;
uniform vec4 position;

void main() {
	gl_Position = vec4(aPos.x + nuevaPosUniform.x, aPos.y + nuevaPosUniform.y, aPos.z + nuevaPosUniform.z, 1.0);

	position = gl_Position;
	myColor = aColor;
}