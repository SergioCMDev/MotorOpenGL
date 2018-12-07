#version 330 core

//in vec3 myColor;
in vec2 textCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float interpolationValue;

out vec4 FragColor; 

void main() {

	FragColor = mix(texture(texture1, textCoord), texture(texture2, textCoord), interpolationValue);


}