#include "Image.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Image::Image() {

}

void Image::AddTextura(char * path, int weight, int  height, int nchannels, char* uniformName)
{
	_widht = weight;
	_height = height;
	_nchannels = nchannels;
	_path = path;
	_uniformName = uniformName;
	_textureData = stbi_load(path
		, &weight, &height, &nchannels, 0);
}

char*Image::GetPath() {
	return _path;
}
char* Image::GetUniformName() {
	return _uniformName;
}
int Image::GetWidht() {
	return _widht;
}
int Image::GetHeight() {
	return _height;
}

int Image::GetNumberOfChannels() {
	return _nchannels;
}

unsigned char * Image::GetTextureData() {
	return _textureData;
}
void Image::ReleaseTexture() {
	stbi_image_free(_textureData);
}



void Image::LoadTexture() {
	uint32_t texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _widht, _height, 0, GL_RGB,
		GL_UNSIGNED_BYTE, _textureData);
	glGenerateMipmap(GL_TEXTURE_2D);
}