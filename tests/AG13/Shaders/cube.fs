#version 330 core

out vec4 FragColor;

in vec3 normal;
in vec3 fragPos;
in vec2 texCoord;
in vec4 fragPosLightSpace;

uniform sampler2D diffuseTexture;
uniform sampler2D depthMap;

uniform vec3 lightPos;
uniform vec3 viewPos;


float ShadowCalculation(vec4 fragPosLightSpace, float bias ){
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	projCoords =  projCoords * 0.5 + 0.5;
	float closestDepth = texture(depthMap, projCoords.xy).r;
	float currentDepth = projCoords.z;

	//float shadow = currentDepth - bias> closestDepth? 1.0 : 0.0; 

	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(depthMap, 0);
	for(int x = -1; x <= 1; ++x){
		for(int y = -1; y <= 1; ++y){
		float pcfDepth = texture(depthMap, projCoords.xy + vec2(x,y) * texelSize).r;
		shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
		}
	}
	shadow /= 9.0;

	if(projCoords.z > 1.0){
		shadow = 0;
	}
	return shadow;
}

void main() {
 	vec3 color = vec3(texture(diffuseTexture, texCoord));
	vec3 normal = normalize(normal);
	vec3 lightColor = vec3(0.3);

	vec3 ambient = 0.3 * color;

	vec3 lightDir = normalize(lightPos - fragPos);
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = diff * lightColor;

	vec3 viewDir = normalize(viewPos - fragPos);

	vec3 halfwayDir = normalize(lightDir +  viewDir);
	float spec =  pow(max(dot(normal, halfwayDir), 0.0), 64);
	vec3 specular = spec * lightColor;

	float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);


	float shadow = ShadowCalculation(fragPosLightSpace, bias);

	vec3 phong = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;
	FragColor = vec4(phong, 1.0);
	
}