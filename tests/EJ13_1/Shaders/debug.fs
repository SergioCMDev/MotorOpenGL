#version 330 core

out vec4 FragColor;
in vec2 texCoords;

uniform sampler2D depthMap;

void main() {
	float depthValue = texture(depthMap, texCoords).r;

	FragColor = vec4(depthValue, depthValue, depthValue, 1.0);
}