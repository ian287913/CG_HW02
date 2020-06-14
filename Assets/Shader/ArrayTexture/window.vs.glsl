#version 410 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texcoord;
//uniform vec4 u_color;
uniform float u_grayScale;
uniform float u_colorScale;

out VS_OUT
{
	vec2 texcoord;
} vs_out;
//out vec4 offsetColor;
out float grayScale;
out float colorScale;

void main(void)
{
	gl_Position = vec4(position, 0.0, 1.0);
	vs_out.texcoord = texcoord;

	//	offset color
	//offsetColor = u_color;
	//	gray
	grayScale = u_grayScale;
	//	color scale
	colorScale = u_colorScale;
}