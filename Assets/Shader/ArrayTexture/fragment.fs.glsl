#version 410 core

layout(location = 0) out vec4 fragColor;

in VertexData
{
	vec2 texcoord;
	flat int spriteIndex;
} vertexData;
in float fadingColor;
in vec4 offsetColor;
in float grayScaleValue;

uniform sampler2DArray tex;

void main()
{
	fragColor = texture(tex, vec3(vertexData.texcoord, vertexData.spriteIndex)) + offsetColor;
	float grayscale_color = 0.299*fragColor.r + 0.587*fragColor.g + 0.114*fragColor.b;
	fragColor = (vec4(grayscale_color, grayscale_color, grayscale_color, fragColor.a) * grayScaleValue) + (fragColor * (1.0f - grayScaleValue));

	//	remove negative offset result
	fragColor.r = fragColor.r < 0 ? 0 : fragColor.r;
	fragColor.g = fragColor.g < 0 ? 0 : fragColor.g;
	fragColor.b = fragColor.b < 0 ? 0 : fragColor.b;

	//	apply fading
	float overkill = 0.0f;
	fragColor.r -= fadingColor;
	fragColor.g -= fadingColor;
	fragColor.b -= fadingColor;

	overkill += fragColor.r < 0.0f ? fragColor.r : 0.0f;
	overkill += fragColor.g < 0.0f ? fragColor.g : 0.0f;
	overkill += fragColor.b < 0.0f ? fragColor.b : 0.0f;

	fragColor.a += overkill;
}