#version 410 core

layout(location = 0) out vec4 fragColor;

in VertexData
{
	vec3 texcoord;
} vertexData;

uniform samplerCube tex;
uniform samplerCubeArray texArray;

void main()
{
	fragColor = texture(tex, vertexData.texcoord);
}