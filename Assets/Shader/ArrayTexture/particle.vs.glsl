#version 410 core

layout(location = 0) in vec2 offset;
layout(location = 1) in int spriteIndex;
layout(location = 2) in float scaling;
layout(location = 3) in float rotation;
layout(location = 4) in vec2 d_vector;

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

vec2 RotateV2(vec2 v2, float radius)
{
	return vec2(v2.x * cos(radius) - v2.y * sin(radius), v2.x * sin(radius) + v2.y * cos(radius));
}

void main()
{
	//vec2 distance = offset + (direction * time * speed);
	vec2 distance = offset + d_vector;

	gl_Position = um4p * um4mv * vec4((RotateV2(points[gl_VertexID], rotation) * scaling) + distance + d_vector, 0.0, 1.0);
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
}
