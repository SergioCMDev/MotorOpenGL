#version 330 core
in vec3 myColor;
in vec2 texCoord;
uniform sampler2D texture1;

out vec4 FragColor; 

void main() {
 		FragColor = texture(texture1, texCoord);

	    //FragColor = vec4(myColor, 1.0);
}