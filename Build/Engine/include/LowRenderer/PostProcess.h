#pragma once

#include <glad/glad.h>
#include "RenderInterface/OpenGL.h"
#include "Resources/IShader.h"
#include "Math.h"
#include "Utils/Engine.h"

class PostProcess
{
public:
    ENGINE_API PostProcess(IShader* shader, int _width, int _height);
    ENGINE_API  void Init();
    ENGINE_API void Resize(int screenWidth, int screenHeight);
    ENGINE_API void Bind();
    ENGINE_API void Unbind();
    ENGINE_API void Render();

private:
    void CreateQuad();

    GLuint fboMSAA = 0;
    GLuint colorBufferMSAA = 0;
    GLuint rboMSAA = 0;

    GLuint fboResolved = 0;
    GLuint textureResolved = 0;
    GLuint depthRBO = 0;


    GLuint quadVAO = 0;
    GLuint quadVBO = 0;

    IShader* shader;

    int width = 0;
    int height = 0;
};
