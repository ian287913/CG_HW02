#version 410 core

layout(location = 0) in vec2 offset;
layout(location = 1) in int spriteIndex;

uniform mat4 um4mv;
uniform mat4 um4p;
uniform float fading;
uniform float grayScale;
uniform vec4 color;

out VertexData
{
	vec2 texcoord;
	flat int spriteIndex;
} vertexData;

out float fadingColor;
out float grayScaleValue;
out vec4 offsetColor;

const vec2[] points = vec2[4](vec2(-0.5, -0.5), vec2(0.5, -0.5), vec2(-0.5, 0.5), vec2(0.5, 0.5));
const vec2[] uv = vec2[4](vec2(0, 0), vec2(1, 0), vec2(0, 1), vec2(1, 1));

void main()
{
	gl_Position = um4p * um4mv * vec4(points[gl_VertexID] + offset, 0.0, 1.0);
	vertexData.texcoord = uv[gl_VertexID];
	vertexData.spriteIndex = spriteIndex;

	//	die fading
	fadingColor = fading;
	//	grayScale
	grayScaleValue = grayScale;
	//	offset color
	offsetColor = color;
}
