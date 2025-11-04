#pragma once
#include "Utils/Engine.h"

#include <vector>
#include <string>
#include <glad/glad.h>
#include <iostream>

#include "stb_image.h"

#include "Math/Math.h"
#include "RenderInterface/OpenGL.h"
#include "Resources/IShader.h"

#include "Camera.h"

class Skybox {
public:
    ENGINE_API Skybox(IShader* _shader);
    ENGINE_API ~Skybox();

    ENGINE_API void Draw(Camera* camera);
    void SetCubemap(unsigned int textureID);
    unsigned int LoadCubemap(const std::string& path);

private:
    unsigned int cubemapTexture;
    unsigned int VAO, VBO;
    IShader* shader;
};