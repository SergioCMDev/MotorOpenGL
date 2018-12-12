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

struct DirLight{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform DirLight dirLight;

struct PointLight{
	vec3 position;

	float constant;
	float linear;
	float cuadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

#define NUMBER_POINT_LIGHTS 2
uniform PointLight pointLight[NUMBER_POINT_LIGHTS];


uniform vec3 viewPos;
out vec4 FragColor; 

vec3 CalcDirectionalLight(DirLight light, vec3 normal, vec3 viewDir){
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, textCoords));

	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * vec3(texture(material.diffuse, textCoords)) * light.diffuse;

	vec3 reflectDir = reflect(-lightDir , normal);
	float spec =  pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = spec * vec3(texture(material.specular, textCoords)) * light.specular;


	return ambient + specular + specular;



}


vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir){

	float distance = length(light.position - fragPos);
	float attenuance = 1.0 / (light.constant + light.linear * distance + light.cuadratic * (distance * distance));
 	
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, textCoords));

	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * vec3(texture(material.diffuse, textCoords)) * light.diffuse;


	vec3 reflectDir = reflect(-lightDir , normal);
	float spec =  pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = spec * vec3(texture(material.specular, textCoords)) * light.specular;

	return (ambient + diffuse + specular) * attenuance;
}


void main() {
	vec3 norm = normalize(normal);
	vec3 viewDir = normalize(viewPos - fragPos);
	
	vec3 color = CalcDirectionalLight(dirLight, norm, viewDir);

	for(int i=0; i < NUMBER_POINT_LIGHTS; ++i){
		color+= CalcPointLight(pointLight[i], norm, fragPos, viewDir);
	}

	FragColor = vec4(color, 1.0);
}