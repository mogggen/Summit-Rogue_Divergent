#pragma once
#include <GL/glew.h>
#include <fstream>
#include "core/MathLibrary.h"

struct ShaderResource
{
	GLchar* vs;
	GLchar* ps;
	GLuint program;

	public:
	~ShaderResource();
	void getShaderResource(GLuint vs, GLuint ps, GLuint program, std::string vsPath="textures/vs.glsl", std::string psPath="textures/ps.glsl");
	void LoadShader(GLchar* vs, GLchar* ps, std::string vsPath, std::string psPath);
	void bindShaderResource();
	
	void setFloat(float32 facIn, std::string parameterName);

	void setV1(float32 floatIn, std::string parameterName);
	void setV2(V2 vecIn, std::string parameterName);
	void setV3(V3 vecIn, std::string parameterName);
	void setV4(V4 vecIn, std::string parameterName);

	void setM4(M4 matIn, std::string parameterName);

};