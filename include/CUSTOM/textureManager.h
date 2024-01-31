#pragma once
#include <map>

class Texture {
public:
	char* name;
	char* path;
	Texture(char* name_, char* path_);

	inline void setTextureSetting(GLint setting, GLint val);
	inline void removeTextureSetting(GLint setting);
	inline void configureAllTextureSettings();

	void use();

private:
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

	bool init();
	inline void bind();
	unsigned char* loadImage();
	void createTexMipmap(unsigned char* data);
};

class TextureManager {
public:
	void initTexture(const char* name_, const char* path_, bool pixelArt = true, bool flipImage = true);
	void useTexture(char* tex, bool setCurrent = true);
private:
	inline void reloadPreviousTexture();
	std::map<const char*, Texture> textures;
	char* currentTexture;
};
