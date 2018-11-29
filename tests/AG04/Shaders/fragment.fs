#version 330 core
in vec3 myColor;
in vec2 textCoord;
out vec4 FragColor; 

uniform sampler2D texture1;
uniform sampler2D texture2;
void main() {
	    FragColor = mix(texture(texture1, textCoord), texture(texture2, textCoord), 0.4);
}