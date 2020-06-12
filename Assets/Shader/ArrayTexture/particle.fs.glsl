#version 410 core

layout(location = 0) out vec4 fragColor;

in VertexData
{
	vec2 texcoord;
	flat int spriteIndex;
} vertexData;

in float fadeCoef;

uniform sampler2DArray tex;

void main()
{
	fragColor = texture(tex, vec3(vertexData.texcoord, vertexData.spriteIndex));
	fragColor.a *= fadeCoef;
}