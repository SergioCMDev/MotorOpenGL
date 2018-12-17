#version 330 core

layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aNormal;

out vec3 gouraud;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMat;

uniform vec3 lightColor;
uniform vec3 objectColor;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform int shininess;

uniform float ambientStrenght;
uniform float specularStrenght;


void main() {
	vec3 normal = normalMat * aNormal;
	vec3 ambient = ambientStrenght * lightColor;

	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightPos - vec3(aPos.x, aPos.y, aPos.z));

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	vec3 viewDir = normalize(viewPos - vec3(aPos.x, aPos.y, aPos.z));
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	vec3 specular = specularStrenght * spec * lightColor;

	gouraud = (ambient + diffuse + specular) * objectColor;


	gl_Position = projection * view * model * vec4(aPos.x, aPos.y,aPos.z,1.0);
}

