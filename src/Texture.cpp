//#include "Texture.h"
//#define STB_IMAGE_IMPLEMENTATION
//#include <stb_image.h>
//
//
//Texture::Texture() {
//
//}
//
//Texture::Texture(const char* path, int weight, int  height, int nchannels, char* uniformName, bool flip) 
//{
//	_widht = weight;
//	_height = height;
//	_nchannels = nchannels;
//	_path = path;
//	_uniformName = uniformName;
//	_textureData = stbi_load(path
//		, &weight, &height, &nchannels, 0);
//
//	stbi_set_flip_vertically_on_load(flip);////}
//
//
//const char*Texture::GetPath() {
//	return _path;
//}
//
//char* Texture::GetUniformName() {
//	return _uniformName;
//}
//int Texture::GetWidht() {
//	return _widht;
//}
//
//int Texture::GetHeight() {
//	return _height;
//}
//
//int Texture::GetNumberOfChannels() {
//	return _nchannels;
//}
//
//unsigned char * Texture::GetTextureData() {
//	return _textureData;
//}
//
// uint32_t Texture::GetTexture() {
//	return _texture;
//}
//
//void Texture::ReleaseTexture() {
//	stbi_image_free(_textureData);
//}
//
//
//
//void Texture::LoadTexture() {
//	glGenTextures(1, &_texture);
//	glBindTexture(GL_TEXTURE_2D, _texture);
//
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _widht, _height, 0, GL_RGB,
//		GL_UNSIGNED_BYTE, _textureData);
//	glGenerateMipmap(GL_TEXTURE_2D);
//}