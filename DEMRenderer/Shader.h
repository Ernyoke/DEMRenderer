#pragma once

#include <string>
#include <fstream>
#include <iostream>

#include "GL\glew.h"

#include "Transform.h"
#include "Camera.h"

class Shader
{
public:
    explicit Shader(const string& fileName);

    //make this class noncopyable
    Shader(const Shader&) = delete;
    Shader operator=(const Shader&) = delete;

    virtual ~Shader();

    void Bind();
    void Update(const Transform &transform, const Camera &camera);

private:

    static const unsigned int NUM_SHADERS = 2;
    static const unsigned int NUM_UNIFORMS = 3;

    static const unsigned int TRAMSFORM_U = 0;
    static const unsigned int LIGHTDIRECTION_U = 1;

    GLuint m_program;
    GLuint m_shaders[NUM_SHADERS];
    GLuint m_uniforms[NUM_UNIFORMS];

    string loadShader(const string& fileName);
    void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const string& errorMessage);
    GLuint CreateShader(const string& text, unsigned int type);
};

