#version 330 core

out vec4 FragColor; 

in vec2 texCoords;

uniform sampler2D textureTree;

void main() {
	vec4 texColor = texture(textureTree, texCoords);
	//if(texColor.a <= 0.1){
	//	discard;
	//}
	 FragColor = texColor;
}