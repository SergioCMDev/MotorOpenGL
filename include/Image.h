#include<glad/glad.h>
#include<GLFW/glfw3.h>


class Image
{
public:
	void Image::AddTextura(char * path, int weight, int  height, int  nchannels, char* uniformName);
	Image();
	unsigned char * GetTextureData();
	char* GetPath();
	char* GetUniformName();
	int GetWidht();
	int GetHeight();
	int GetNumberOfChannels();
	void LoadTexture();
	void ReleaseTexture();
private:
	unsigned char *_textureData;
	char* _path, *_uniformName;
	int _widht, _height, _nchannels;

};