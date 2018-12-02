#version 330 core
in vec3 myColor;

out vec4 FragColor; 

uniform vec3 myColorUniform;
void main() {
	    FragColor = vec4(myColorUniform, 1.0);
}