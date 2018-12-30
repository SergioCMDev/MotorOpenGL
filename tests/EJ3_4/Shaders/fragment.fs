#version 330 core
in vec3 myColor;
in vec3 pos;

out vec4 FragColor; 
out vec3 fragPos; 

void main() {
	 fragPos = pos;
	 FragColor = vec4(myColor, 1.0);
}