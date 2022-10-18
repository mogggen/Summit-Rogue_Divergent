#version 300 es
#extension GL_EXT_separate_shader_objects : require
precision highp float;
layout(location=0) in vec4 pos;
layout(location=1) in vec4 color;

out vec4 colorOut;

uniform mat4 matrix;
void main()
{
	gl_Position = matrix * pos;
	colorOut = color;
}