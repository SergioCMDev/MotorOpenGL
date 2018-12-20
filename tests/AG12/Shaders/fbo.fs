#version 330 core


in vec2 texCoords;


out vec4 FragColor;

uniform sampler2D  screenTexture;

void main() {
vec2 offsets[9] = vec[]{
	vec2(-offset, offset)
	vec2(-0.0, offset)
	vec2(-offset, offset)
	vec2(-offset, 0.0)
	vec2(0.0, 0.0)
	vec2(-offset, 0.0)
	vec2(0.0, -offset)
	vec2(-offset, offset)
};

float kernet[9] ) float[]{
	-1,-1,-1
	-1,9,-1,
	-1,-1,-1
};

vec3 sampleTex[9];
for(int i = 0; i< 9; ++i){
	sampleTex[i] = vec3(texture(screenTexture, texCoords.st + offset[i]));
}

vec3 color = vec3(0.0);
for(int i = 0; i< 9; ++i){
	color +=sampleTex[i] * kernel[i];
}

//vec3 color = texture(screenTexture, texCoords);
//float average = 0.2126 *color.r + 0.75152 * color.g +0.0722*color.b;
 		//FragColor = vec4(average, average,average, 1.0);
 		FragColor = vec4(col, 1.0);
}