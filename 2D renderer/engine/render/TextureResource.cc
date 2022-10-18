#include "config.h"
#include "render/TextureResource.h"

TextureResource::TextureResource(std::string file) : file(file)
{
	
}

void TextureResource::LoadFromFile()
	{
		int imgWidth, imgHeight, nrChannels;

		unsigned char* img = stbi_load(file.c_str(), &imgWidth, &imgHeight, &nrChannels, STBI_rgb);
		if (img == nullptr)
		{
			printf("Image loaded incorrectly");
			exit(1);
		}

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		if (nrChannels == 3)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
		}

		else if (nrChannels == 4)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
		}

		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

void TextureResource::BindTexture()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
}

TextureResource::~TextureResource()
{
	glDeleteTextures(1, &texture);
}