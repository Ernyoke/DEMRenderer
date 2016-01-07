#include "stdafx.h"
#include "Shader.h"


Shader::Shader(const string& fileName) {
	m_program = glCreateProgram();

	m_shaders[0] = createShader(loadShader(fileName + ".vs"), GL_VERTEX_SHADER);
	m_shaders[1] = createShader(loadShader(fileName + ".fs"), GL_FRAGMENT_SHADER);

	for (unsigned int i = 0; i < NUM_SHADERS; ++i) {
		glAttachShader(m_program, m_shaders[i]);
	}

	glBindAttribLocation(m_program, 0, "position");

	glLinkProgram(m_program);
	checkShaderError(m_program, GL_LINK_STATUS, true, "Program linking failed!");

	glValidateProgram(m_program);
	checkShaderError(m_program, GL_VALIDATE_STATUS, true, "Program is invalid!");

	m_uniforms[TRAMSFORM_U] = glGetUniformLocation(m_program, "transform");
}


Shader::~Shader() {
	for (unsigned int i = 0; i < NUM_SHADERS; ++i) {
		glDetachShader(m_program, m_shaders[i]);
		glDeleteShader(m_shaders[i]);
	}
	glDeleteProgram(m_program);
}

void Shader::bind() {
	glUseProgram(m_program);
}

void Shader::update(const Transform &transform, const Camera &camera) {
	mat4 model = camera.getViewProjection() * transform.getModel();
	glUniformMatrix4fv(m_uniforms[TRAMSFORM_U], 1, GL_FALSE, &model[0][0]);
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

void Shader::checkShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage) {
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

GLuint Shader::createShader(const string& text, unsigned int type) {
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

	checkShaderError(m_program, GL_COMPILE_STATUS, false, "Shader compilation failed!");

	return shader;
}
