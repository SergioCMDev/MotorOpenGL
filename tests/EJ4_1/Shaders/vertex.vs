#version 330 core

layout (location=1) in vec3 aColor;
layout (location=0) in vec3 aPos;
layout (location=2) in vec2 aTexCoord;

out vec2 textCoord;
out vec3 myColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec4 nuevaPosUniform;


void main() {
	 //gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);

	gl_Position = projection * view * model * vec4(aPos.x + nuevaPosUniform.x, aPos.y + nuevaPosUniform.y, aPos.
	z + nuevaPosUniform.z, 1.0);

	// gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);

	textCoord = vec2(aTexCoord.x, aTexCoord.y);
	myColor = aColor;
}

