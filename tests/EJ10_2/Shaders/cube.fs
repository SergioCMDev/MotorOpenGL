#version 330 core

uniform vec3 color;
uniform float transparencia;
out vec4 FragColor;
void main() {
 		FragColor = vec4(color, transparencia);
}