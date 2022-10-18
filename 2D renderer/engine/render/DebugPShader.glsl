#version 300 es
#extension GL_EXT_separate_shader_objects : require
precision highp float;
in vec4 colorOut;

out vec4 Color;

void main()
{
	Color = colorOut;
}