#include "config.h"
#include "render/ShaderResource.h"
#include <cstring>

//filetype: .glsl
void ShaderResource::LoadShader(GLchar *vs, GLchar *ps, std::string vsPath, std::string psPath)
{
	std::streampos size;
	//vs
	std::ifstream pathVS(vsPath, std::ios::in | std::ios::binary | std::ios::ate);
	if (pathVS.is_open())
	{
		size = pathVS.tellg();
		vs = new GLchar[size + std::streampos(1)];
		pathVS.seekg(0, std::ios::beg);
		pathVS.read(vs, size);
		vs[size] = '\0';
		pathVS.close();
	}

	//ps
	std::ifstream pathPS(psPath, std::ios::in | std::ios::binary | std::ios::ate);
	if (pathPS.is_open())
	{
		size = pathPS.tellg();
		ps = new GLchar[size + std::streampos(1)];
		pathPS.seekg(0, std::ios::beg);
		pathPS.read(ps, size);
		ps[size] = '\0';
		pathPS.close();
	}

	this->vs = vs;
	this->ps = ps;
}

void ShaderResource::getShaderResource(GLuint vertexShader, GLuint pixelShader, GLuint program, std::string vsPath, std::string psPath)
{
	LoadShader(vs, ps, vsPath, psPath);

	// setup vertex shader
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLint length = static_cast<GLint>(std::strlen(vs));
	glShaderSource(vertexShader, 1, &vs, &length);
	glCompileShader(vertexShader);

	// get error log
	GLint shaderLogSize;
	glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &shaderLogSize);
	if (shaderLogSize > 0)
	{
		GLchar *buf = new GLchar[shaderLogSize];
		glGetShaderInfoLog(vertexShader, shaderLogSize, NULL, buf);
		printf("[SHADER COMPILE ERROR]: %s", buf);
		delete[] buf;
	}

	// setup pixel shader
	pixelShader = glCreateShader(GL_FRAGMENT_SHADER);
	length = static_cast<GLint>(std::strlen(ps));
	glShaderSource(pixelShader, 1, &ps, &length);
	glCompileShader(pixelShader);

	// get error log
	glGetShaderiv(pixelShader, GL_INFO_LOG_LENGTH, &shaderLogSize);
	if (shaderLogSize > 0)
	{
		GLchar *buf = new GLchar[shaderLogSize];
		glGetShaderInfoLog(pixelShader, shaderLogSize, NULL, buf);
		printf("[SHADER COMPILE ERROR]: %s", buf);
		delete[] buf;
	}

	// create a program object
	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, pixelShader);
	glLinkProgram(program);
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &shaderLogSize);
	if (shaderLogSize > 0)
	{
		GLchar *buf = new GLchar[shaderLogSize];
		glGetProgramInfoLog(program, shaderLogSize, NULL, buf);
		printf("[PROGRAM LINK ERROR]: %s", buf);
		delete[] buf;
	}
	this->program = program;
}

void ShaderResource::setFloat(float32 facIn, std::string parameterName)
{
	glUseProgram(program);
	glUniform1f(glGetUniformLocation(program, parameterName.c_str()), facIn);
}

void ShaderResource::setV1(float32 vecIn, std::string parameterName)
{
	glUseProgram(program);
	glUniform1fv(glGetUniformLocation(program, parameterName.c_str()), 1, (float32*)&vecIn);
}

void ShaderResource::setV2(V2 vecIn, std::string parameterName)
{
	glUseProgram(program);
	glUniform2fv(glGetUniformLocation(program, parameterName.c_str()), 1, (float32*)&vecIn);
}

void ShaderResource::setV3(V3 vecIn, std::string parameterName)
{
	glUseProgram(program);
	glUniform3fv(glGetUniformLocation(program, parameterName.c_str()), 1, (float32*)&vecIn);
}

void ShaderResource::setV4(V4 vecIn, std::string parameterName)
{
	glUseProgram(program);
	glUniform4fv(glGetUniformLocation(program, parameterName.c_str()), 1, (float32*)&vecIn);
}

void ShaderResource::setM4(M4 matIn, std::string parameterName)
{
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, parameterName.c_str()), 1, GL_FALSE, (float32*)&(matIn));
}

void ShaderResource::bindShaderResource()
{
	glUseProgram(program);
}

ShaderResource::~ShaderResource()
{
	glDeleteProgram(program);
}