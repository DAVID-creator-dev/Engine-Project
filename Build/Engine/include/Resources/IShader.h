#pragma once
#include "Math/Math.h"

#ifdef APIENTRY
#undef APIENTRY
#endif
#include <glad/glad.h>

class ShaderGL;

class IShader
{
public:
    virtual ~IShader() = default;
    virtual void CreateShader(const char* vertexPath, const char* fragmentPath) = 0;

    virtual GLuint GetProgram() = 0; 

    virtual ShaderGL* GetGL() = 0;
};