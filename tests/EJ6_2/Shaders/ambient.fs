#version 330 core

uniform vec3 objectColor;
unirform vec3 lightColor;
uniform float ambientStrengh;

void main() {
	vec3 ambient = ambientStrengh * lightColor;
	vec3 phong = ambientStrengh * objectColor;
	FragColor = vec4(phong, 1.0);
}