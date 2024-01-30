#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <OTHER/stb_image.h>

#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <map>

#include <CUSTOM/loadTexture.h>

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

class textureManager {
    public:
    void initTexture(const char* path);
    void useTexture(const char* tex);

    private:
        unsigned int loadTexture(const char* texturePath, bool flipImage = true);

        std::map<const char*, unsigned int> textures;
};