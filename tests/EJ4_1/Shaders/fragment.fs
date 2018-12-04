#version 330 core

in vec3 myColor;
in vec2 textCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

out vec4 FragColor; 

void main() {
 	//FragColor = texture(texture1, textCoord);
	//FragColor = texture(texture2, textCoord);
	FragColor = mix(texture(texture1, textCoord), texture(texture2, textCoord), 0.6);

	//FragColor = vec4(myColor, 1.0);
}