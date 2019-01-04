#version 330 core

in vec2 textCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform vec3 color;


out vec4 FragColor; 

void main() {
	FragColor = texture(texture1, textCoord);
}