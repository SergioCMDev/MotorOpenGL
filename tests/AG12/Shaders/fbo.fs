#version 330 core

in vec2 texCoords;

out vec4 FragColor;

uniform sampler2D screenTexture;

//kernel 1
const float offset = 1.0 / 300.0;


void main() {
//Valor inicial
//FragColor = texture(screenTexture, texCoords);

//PostProceso1, Mostrar inverso del color
//FragColor = 1 - texture(screenTexture, texCoords);

//Mostrar escala de grises
//vec4 color = texture(screenTexture, texCoords);
//float average = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
//FragColor = vec4(average, average, average, 1);


//kernel 1
vec2 offsets[9] = vec2[] (
	vec2(-offset, offset),	
	vec2(0.0, offset),	
	vec2(offset, offset),	
	vec2(-offset, 0.0),	
	vec2(0.0, 0.0),	
	vec2(offset, 0.0),	
	vec2(-offset, -offset),	
	vec2(0.0, -offset),	
	vec2(offset, -offset)	
);

//Kernel con ruido
float kernel[9] = float[](
	-1,-1,-1,
	-1,9,-1,
	-1,-1,-1
);

//Kernel con blurr
float kernel2[9] = float[](
	1.0/16, 2.0/16, 1.0/16,
	2.0/16, 4.0/16, 2.0/16,
	1.0/16, 2.0/16, 1.0/16
);


//Kernel de bordes
float kernel3[9] = float[](
	1,1,1,
	1,-8,1,
	1,1,1
);

vec3 sampleText[9];
for (int i = 0; i < 9; ++i)
{
	sampleText[i] = vec3(texture(screenTexture, texCoords.st + offsets[i] ));
}
vec3 color = vec3(0.0);
for (int i = 0; i < 9; ++i)
{
	color+= sampleText[i] * kernel3[i];
}
FragColor = vec4(color, 1);

}