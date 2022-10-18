#pragma once
#include <GL/glew.h>
#include "stb_image.h"
#include <string>

class TextureResource
{
	GLuint texture;
	std::string file;
public:
	TextureResource(std::string file);
	~TextureResource();
	void BindTexture();
	void LoadFromFile();
};