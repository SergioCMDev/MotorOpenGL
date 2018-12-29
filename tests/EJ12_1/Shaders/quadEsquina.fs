#version 330 core

in vec2 texCoords;

out vec4 FragColor;

uniform sampler2D screenTexture;


void main() {
//Valor inicial
	FragColor = texture(screenTexture, texCoords);

}