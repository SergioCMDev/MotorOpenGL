#version 330 core

in vec3 normal;
in vec3 fragPos;
in vec2 textCoords;

struct Material{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};
uniform Material material;

struct Light{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform Light light;
uniform vec3 viewPos;


out vec4 FragColor; 

void main() {
 	
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, textCoords));

	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * vec3(texture(material.diffuse, textCoords)) * light.diffuse;

	vec3 viewDir = normalize(viewPos - fragPos);

	vec3 reflectDir = reflect(-lightDir , norm);
	float spec =  pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = spec * vec3(texture(material.specular, textCoords)) * light.specular;

	vec3 phong = ambient + diffuse + specular ;
	FragColor = vec4(phong, 1.0);
	
}