#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <OTHER/stb_image.h>

#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <map>

#include <CUSTOM/textureManager.h>

unsigned int loadTexture(const char* texturePath, bool flipImage)
{
    stbi_set_flip_vertically_on_load(flipImage);

    // Do something
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Configure the texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // GL_REPEAT
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    
    //if (renderSettings.pixelArt)
    if (true) // Temporarily always use pixel-art-style scaling and rendering of textures
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }


    // Read the image from file
    int width, height, nrChannels;
    unsigned char* data = stbi_load(texturePath, &width, &height, &nrChannels, STBI_rgb_alpha);

    if (data)
    {
        // Create the texture and its mipmap
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        std::cout << "SUCCESS loading texture : " << texturePath << std::endl;
    }
    else
    {
        std::cout << "FAILED to load texture : " << texturePath << std::endl;
    }

    // Free the image memory
    stbi_image_free(data);

    return texture;
}

//class TextureManager{
//public:
void TextureManager::initTexture(const char* name_, const char* path_, bool pixelArt, bool flipImage)
{
    char* name = (char*)name_;
    char* path = (char*)path_;

    std::cout << "Initialising texture " << name << std::endl;

    Texture* tex = new Texture(name, path, pixelArt, flipImage);
    textures[name] = tex;

    reloadPreviousTexture();
} 


void TextureManager::useTexture(char* tex, bool setCurrent)
{
    if (tex == currentTexture) { return; }
    previousTexture = currentTexture;
    textures[tex]->use();
    currentTexture = tex;
}

//private:
inline void TextureManager::reloadPreviousTexture()
{
    if (previousTexture) { useTexture(previousTexture); }
}


// ######################################################################## \\ 

// Texture class


// Constructor
Texture::Texture(char* name_, char* path_, bool pixelArt_, bool flipImage_)
{
    name = name_;
    path = path_;
    pixelArt = pixelArt_;
    flipImage = flipImage_;

    init();
}

// Rendering settings for the texture
    
inline void Texture::setTextureSetting(GLint setting, GLint val)  { textureSettings[setting] = val; }
inline void Texture::removeTextureSetting(GLint setting)          { textureSettings.erase(setting); }
inline void Texture::clearTextureSettings()                       { textureSettings.clear(); }
inline void Texture::configureAllTextureSettings()
{
    std::cout << "\nConfiguring all texture settings for texture " << name << std::endl;
    for (const auto p : textureSettings)
    {
        glTexParameteri(GL_TEXTURE_2D, p.first, p.second);
        std::cout << "> " << p.first << " > " << p.second << std::endl;
    }
    std::cout << std::endl;
}
void Texture::initSettings()
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

// Use texture
inline void Texture::use()
{
    bind();
}

//private:
bool Texture::init()
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

inline void Texture::bind()
{
    glBindTexture(GL_TEXTURE_2D, texture);
}


unsigned char* Texture::loadImage()
{
    stbi_set_flip_vertically_on_load(flipImage);
    // Read the image from file
    unsigned char* data = stbi_load(path, &imageInfo.width, &imageInfo.height, &imageInfo.nrChannels, STBI_rgb_alpha);

    if (data)
    {
        imageLoadSuccess = true;
        std::cout << "SUCCESS loading texture : " << path << std::endl;
    }
    else
    {
        imageLoadSuccess = false;
        std::cout << "FAILED to load texture : " << path << std::endl;
    }
    
    return data;
}

void Texture::createTexMipmap(unsigned char* data)
{
    // Create the texture and its mipmap
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageInfo.width, imageInfo.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    mipmapGenerated = true;
}