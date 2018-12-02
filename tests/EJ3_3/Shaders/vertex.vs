#version 330 core
layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aColor;
out vec3 myColor;
uniform vec3 nuevaPosUniform;

void main() {
	 gl_Position = vec4(aPos.x + nuevaPosUniform.x, aPos.y + nuevaPosUniform.y, aPos.z + nuevaPosUniform.z, 1.0);
	 myColor = aColor;
}