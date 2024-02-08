#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <OTHER/stb_image.h>

#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <map>

#include <OTHER/termcolor.hpp>

#ifndef TextureManagerHeader
#define TextureManagerHeader

class Texture {
public:
	const char* name;
	const char* path;
	
	Texture(const char* name_, const char* path_, bool pixelArt_, bool flipImage_)
	{
		name = name_;
		path = path_;
		pixelArt = pixelArt_;
		flipImage = flipImage_;

		init();
	}

	inline void setTextureSetting(GLint setting, GLint val)  { textureSettings[setting] = val; }
	inline void removeTextureSetting(GLint setting)          { textureSettings.erase(setting); }
	inline void configureAllTextureSettings()                { for (const auto p : textureSettings) { glTexParameteri(GL_TEXTURE_2D, p.first, p.second); } }
	inline void clearTextureSettings()                       { textureSettings.clear(); }

	inline void use() { bind(); }

private:
	bool pixelArt;
	bool flipImage;
	bool mipmapGenerated = false;
	unsigned int texture;
	bool imageLoadSuccess;
	//std::pair<int, int> imageDimensions;
	struct {
		int width;
		int height;
		int nrChannels;
	} imageInfo;

	std::map<GLint, GLint> textureSettings;

	bool init()
	{
		glGenTextures(1, &texture);
		bind();
		initSettings();
		{

			unsigned char* d = loadImage();
			if (!imageLoadSuccess) { return false; }
			createTexMipmap(d);
		}
		return true;
	}

	inline void bind()
	{
		glBindTexture(GL_TEXTURE_2D, texture);
	}

	unsigned char* loadImage()
	{
		stbi_set_flip_vertically_on_load(flipImage);
		// Read the image from file
		unsigned char* data = stbi_load(path, &imageInfo.width, &imageInfo.height, &imageInfo.nrChannels, STBI_rgb_alpha);

		if (data)
		{
			imageLoadSuccess = true;
			std::cout << colour::green << "SUCCESS loading texture : " << path << colour::reset << std::endl;
		}
		else
		{
			imageLoadSuccess = false;
			std::cout << colour::red << "FAILED to load texture : " << path << colour::reset << std::endl;
		}

		return data;
	}

	void createTexMipmap(unsigned char* data)
	{
		if (mipmapGenerated) { return; }
		// Create the texture and its mipmap
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageInfo.width, imageInfo.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		mipmapGenerated = true;
	}

	void initSettings()
	{

		setTextureSetting(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // GL_REPEAT
		setTextureSetting(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		if (pixelArt)
		{
			setTextureSetting(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			setTextureSetting(GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		}
		else
		{
			setTextureSetting(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			setTextureSetting(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		}

		configureAllTextureSettings();

	}
};

class TextureManager {
public:
	void initTexture(const char* name_, const char* path_, bool pixelArt = true, bool flipImage = true)
	{
		Texture* tex = new Texture(name_, path_, pixelArt, flipImage);
		textures[name_] = tex;

		reloadPreviousTexture();
	}
	void useTexture(const char* tex, bool setCurrent = true)
	{
		if (tex == currentTexture) { return; }
		previousTexture = currentTexture;
		textures[tex]->use();
		currentTexture = tex;
	}
private:
	inline void reloadPreviousTexture()
	{
		if (previousTexture) { useTexture(previousTexture); }
	}
	std::map<const char*, Texture*> textures;
	const char* currentTexture = "None";
	const char* previousTexture = NULL;
};

#endif // TextureManagerHeader