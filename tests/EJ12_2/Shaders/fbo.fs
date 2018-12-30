#version 330 core

in vec2 texCoords;

out vec4 FragColor;

uniform sampler2D screenTexture;



void main() {
//Valor inicial
//FragColor = texture(screenTexture, texCoords);


//Kernel
float Gx[9] = float[](
	1, 0, -1,
	2, 0, -2,
	1, 0, -1
);

float Gy[9] = float[](
	1,  2, 1,
	0, 0, 0,
	-1, -2, -1
);

//float S1, S2;
//for (int i = 0; i < 9; ++i)
//{
//	for (int j = 0; j <9; ++j)
//	{
//		S1 += Gx[i][j] * screenTexture[i][j];
//		S2 += Gy[i][j] * screenTexture[i][j];
//	}
//}
vec3 sampleText[9];

for (int i = 0; i < 9; ++i)
{
	sampleText[i] = vec3(texture(screenTexture, texCoords.st));
}
float color1;
for (int i = 0; i < 9; ++i)
{
	color1+= sampleText[i] * Gx[i];
}
float color2;

for (int i = 0; i < 9; ++i)
{
	color2+= sampleText[i] * Gy[i];
}

float color = sqrt(pow(color1, 2.0)+pow(color2, 2.0));

FragColor = vec4(vec3(color), 1);

}