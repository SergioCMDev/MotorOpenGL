#version 330 core

out vec4 FragColor; 

uniform vec3 myColorUniform;
void main() {
	    FragColor = vec4(myColorUniform, 1.0);
}