#version 330 core

in vec2 textCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

out vec4 FragColor; 

void main() {

	FragColor = mix(texture(texture1, textCoord), 
					texture(texture2, vec2(textCoord.x, 1-textCoord.y)), 0.6);


}