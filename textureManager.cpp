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

class TextureManager{
public:
    void initTexture(const char* name_, const char* path_, bool pixelArt = true, bool flipImage = true)
    {
        char* name = (char*)name_;
        char* path = (char*)path_;
        
        unsigned int texture;

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);


        // Configure the texture
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // GL_REPEAT
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        if (pixelArt)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        }
        else
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }


        // Read the image from file
        int width, height, nrChannels;
        unsigned char* data = stbi_load(path, &width, &height, &nrChannels, STBI_rgb_alpha);

        if (data)
        {
            // Create the texture and its mipmap
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            std::cout << "SUCCESS loading texture : " << path << std::endl;
        }
        else
        {
            std::cout << "FAILED to load texture : " << path << std::endl;
        }

        // Add the texture to the texture manager's collection
        Texture textureObj(name, path);
        textures[name] = textureObj;

        // Go back to the previous texure, if there was one
        reloadPreviousTexture();
    }


    void useTexture(char* tex, bool setCurrent = true)
    {

    }

private:
    inline void reloadPreviousTexture()
    {
        useTexture(currentTexture);
    }

    std::map<const char*, Texture> textures;
    char* currentTexture;
};

class Texture
{
public:
    char* name;
    char* path;

    // Constructor
    Texture(char* name_, char* path_)
    {
        name = name_;
        path = path_;

        init();
    }

    // Rendering settings for the texture
    
    inline void setTextureSetting(GLint setting, GLint val)  { textureSettings[setting] = val; }
    inline void removeTextureSetting(GLint setting)          { textureSettings.erase(setting); }
    inline void configureAllTextureSettings()
    {
        for (const auto p : textureSettings)
        {
            glTexParameteri(GL_TEXTURE_2D, p.first, p.second);
        }
    }

    // Use texture
    void use()
    {
        
    }

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

    bool init()
    {
        glGenTextures(1, &texture);
        bind();
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

    void createTexMipmap(unsigned char* data)
    {
        // Create the texture and its mipmap
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageInfo.width, imageInfo.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        mipmapGenerated = true;
    }
    
};