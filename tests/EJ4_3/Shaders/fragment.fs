#version 330 core
in vec3 myColor;
in vec4 position;

out vec4 FragColor; 

void main() {

	    FragColor = vec4(myColor, 1.0);
}