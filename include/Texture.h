#ifndef TEXTURE_H
#define TEXTURE_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>


class Texture
{
public:
	Texture();
	Texture::Texture(const char* path, int weight, int  height, int nchannels, char* uniformName, bool flip);
	unsigned char * GetTextureData();
	const char* GetPath();
	char* GetUniformName();
	int GetWidht();
	int GetHeight();
	int GetNumberOfChannels();
	uint32_t GetTexture();
	void LoadTexture();
	void ReleaseTexture();
private:
	unsigned char *_textureData;
	const char* _path;
	char*_uniformName;
	int _widht, _height, _nchannels;
	uint32_t _texture;
};
#endif //