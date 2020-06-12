#version 410 core

layout(location = 0) out vec4 fragColor;

in VertexData
{
	vec2 texcoord;
	flat int spriteIndex;
} vertexData;
in float fadingColor;

uniform sampler2DArray tex;

void main()
{
	fragColor = texture(tex, vec3(vertexData.texcoord, vertexData.spriteIndex));
	float overkill = 0.0f;
	fragColor.r -= fadingColor;
	fragColor.g -= fadingColor;
	fragColor.b -= fadingColor;

	overkill += fragColor.r < 0.0f ? fragColor.r : 0.0f;
	overkill += fragColor.g < 0.0f ? fragColor.g : 0.0f;
	overkill += fragColor.b < 0.0f ? fragColor.b : 0.0f;

	fragColor.a += overkill;
}