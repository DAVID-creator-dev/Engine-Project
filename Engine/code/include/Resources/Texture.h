#pragma once
#include "ITexture.h"
#include <glad/glad.h>

class TextureGL : public ITexture
{
public:
    TextureGL() = default;
    ~TextureGL() override;

    void LoadResource(const char* path, const ResourceLoadParams& params) override;

    TextureGL* GetGL() override { return this; }
    GLuint GetTextureID() const override { return textureID; }

private:
    GLuint textureID;
};