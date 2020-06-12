#version 410 core

layout(location = 0) in vec2 offset;
layout(location = 1) in int spriteIndex;

uniform mat4 um4mv;
uniform mat4 um4p;
uniform float time;
uniform float speed;
uniform float fadeRadius;
uniform float fadeDistance;

out VertexData
{
	vec2 texcoord;
	flat int spriteIndex;
} vertexData;

out float fadeCoef;

const vec2[] points = vec2[4](vec2(-0.5, -0.5), vec2(0.5, -0.5), vec2(-0.5, 0.5), vec2(0.5, 0.5));
const vec2[] uv = vec2[4](vec2(0, 0), vec2(1, 0), vec2(0, 1), vec2(1, 1));

void main()
{
	vec2 distance = offset * (1.0f + time * speed);

	gl_Position = um4p * um4mv * vec4(points[gl_VertexID] + distance, 0.0, 1.0);
	vertexData.texcoord = uv[gl_VertexID];
	vertexData.spriteIndex = spriteIndex;

	//	calc fade coeficient
	if (fadeDistance != 0)
	{
		fadeCoef = 1.0f - (length(distance) - fadeRadius) / fadeDistance;
	}
	else
	{
		fadeCoef = fadeRadius - length(distance);
	}
	if (fadeCoef >= 1)
		fadeCoef = 1;
	else if (fadeCoef < 0)
		fadeCoef = 0;

	//fadeCoef = 1;
}
