#include "stdafx.h"
#include "Shader.h"


Shader::Shader(const string& fileName) {
    m_program = glCreateProgram();

    m_shaders[0] = CreateShader(loadShader(fileName + ".vs"), GL_VERTEX_SHADER);
    m_shaders[1] = CreateShader(loadShader(fileName + ".fs"), GL_FRAGMENT_SHADER);

    for (unsigned int i = 0; i < NUM_SHADERS; ++i) {
        glAttachShader(m_program, m_shaders[i]);
    }

    glBindAttribLocation(m_program, 0, "position");
    glBindAttribLocation(m_program, 1, "normal");

    glLinkProgram(m_program);
    CheckShaderError(m_program, GL_LINK_STATUS, true, "Program linking failed!");

    glValidateProgram(m_program);
    CheckShaderError(m_program, GL_VALIDATE_STATUS, true, "Program is invalid!");

    m_uniforms[0] = glGetUniformLocation(m_program, "MVP");
    m_uniforms[1] = glGetUniformLocation(m_program, "Normal");
    m_uniforms[2] = glGetUniformLocation(m_program, "lightDirection");
}


Shader::~Shader() {
    for (unsigned int i = 0; i < NUM_SHADERS; ++i) {
        glDetachShader(m_program, m_shaders[i]);
        glDeleteShader(m_shaders[i]);
    }
    glDeleteProgram(m_program);
}

void Shader::Bind() {
    glUseProgram(m_program);
}

void Shader::Update(const Transform &transform, const Camera &camera) {
    glm::mat4 MVP = camera.GetViewProjection() * transform.GetModel();
    glm::mat4 Normal = transform.GetModel();
    glUniformMatrix4fv(m_uniforms[0], 1, GL_FALSE, &MVP[0][0]);
    glUniformMatrix4fv(m_uniforms[1], 1, GL_FALSE, &Normal[0][0]);
    glUniform3f(m_uniforms[2], 0.0f, 0.0f, 1.0f);
}

string Shader::loadShader(const std::string& fileName) {
    std::ifstream file;
    file.open((fileName).c_str());

    std::string output;
    std::string line;

    if (file.is_open())
    {
        while (file.good())
        {
            getline(file, line);
            output.append(line + "\n");
        }
    }
    else
    {
        cerr << "Unable to load shader: " << fileName << endl;
    }

    return output;
}

void Shader::CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage) {
    GLint success = 1;
    GLchar error[1024] = { 0 };

    if (isProgram) {
        glGetProgramiv(shader, flag, &success);
    }
    else {
        glGetShaderiv(shader, flag, &success);
    }

    if (success == GL_FALSE) {
        if (isProgram) {
            glGetProgramInfoLog(shader, sizeof(error), NULL, error);
        }
        else {
            glGetShaderInfoLog(shader, sizeof(error), NULL, error);
        }
        cerr << errorMessage << ": '" << error << "'" << endl;
    }
}

GLuint Shader::CreateShader(const string& text, unsigned int type) {
    GLuint shader = glCreateShader(type);
    if (shader == 0) {
        cerr << "Shader creation failed!" << endl;
    }

    const GLchar* shaderString[1];
    GLint shaderStringLength[1];

    shaderString[0] = text.c_str();
    shaderStringLength[0] = text.length();

    glShaderSource(shader, 1, shaderString, shaderStringLength);
    glCompileShader(shader);

    CheckShaderError(m_program, GL_COMPILE_STATUS, false, "Shader compilation failed!");

    return shader;
}
