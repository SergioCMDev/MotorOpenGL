#version 330 core

out vec4 FragColor;

in vec3 normal;
in vec2 TexCoords;
in vec3 fragPos;


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
uniform PointLight pointLights;

struct SpotLight{
	vec3 position;
	vec3 direction;

	float cutOff;
	float outerCutOff;

	float linear;
	float constant;
	float cuadratic;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform SpotLight spotLight;

struct Material{
	float shininess;
};
uniform Material material;

uniform vec3 viewPos;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;


vec3 CalcDirectionalLight(DirLight light, vec3 normal, vec3 viewDir){
	vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, TexCoords));

	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * vec3(texture(texture_diffuse1, TexCoords)) * light.diffuse;

	vec3 reflectDir = reflect(-lightDir , normal);
	float spec =  pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = spec * vec3(texture(texture_specular1, TexCoords)) * light.specular;


	return ambient + specular + specular;
}


vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir){

	float distance = length(light.position - fragPos);
	float attenuance = 1.0 / (light.constant + light.linear * distance + light.cuadratic * (distance * distance));
 	
	vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, TexCoords));

	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * vec3(texture(texture_diffuse1, TexCoords)) * light.diffuse;


	vec3 reflectDir = reflect(-lightDir , normal);
	float spec =  pow(max(dot(viewDir, reflectDir), 0.0),material.shininess);
	vec3 specular = spec * vec3(texture(texture_specular1, TexCoords)) * light.specular;

	return (ambient + diffuse + specular) * attenuance;
}

vec3 calcSpotLight(SpotLight light){
	float distance = length(light.position - fragPos);
	float attenuance = 1.0 / (light.constant + light.linear * distance + light.cuadratic * (distance * distance));
 	
	vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, TexCoords));

	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * vec3(texture(texture_diffuse1, TexCoords)) * light.diffuse;

	vec3 viewDir = normalize(viewPos - fragPos);

	vec3 reflectDir = reflect(-lightDir , norm);
	float spec =  pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = spec * vec3(texture(texture_specular1, TexCoords)) * light.specular;

	float theta =dot(lightDir, normalize(-light.direction));
	float epsilon =light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0,1.0);

	vec3 phong = (ambient + (diffuse * intensity) + (specular * intensity)) * attenuance;
	return phong;
}

void main() {
	vec3 norm = normalize(normal);
	vec3 viewDir = normalize(viewPos - fragPos);

	vec3 color = CalcDirectionalLight(dirLight, norm, viewDir);

	//vec3 color = CalcPointLight(pointLights, norm, viewDir);
	color += CalcPointLight(pointLights, norm, viewDir);

	//vec3 color = calcSpotLight(spotLight);
	//color += calcSpotLight(spotLight);


	//FragColor = texture(texture_diffuse1, TexCoords);
	//FragColor = texture(texture_specular1, TexCoords);
	FragColor = vec4(color, 1.0) ;
	//vec3 rgb_normal = normal * 0.5 + 0.5; // transforms from [-1,1] to [0,1]  

	//FragColor = vec4(rgb_normal, 1.0);

}