#version 330 core
layout (location=1) in vec3 aColor;
out vec3 myColor;
uniform vec3 nuevaPos;

void main() {
	 gl_Position = vec4(nuevaPos.x,nuevaPos.y,nuevaPos.z, 1.0);
	 myColor = aColor;
}