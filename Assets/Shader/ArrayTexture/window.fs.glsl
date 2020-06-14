#version 410 core                                                              

uniform sampler2D tex;                                                         

out vec4 color;                                                                

in VS_OUT                                                                      
{                                                                              
vec2 texcoord;                                                             
} fs_in;                                                                       
in float grayScale;
in float colorScale;
//in vec4 offsetColor;

void main(void)                                                                
{
	//	apply grayScale
	vec4 texture_color = texture(tex, fs_in.texcoord);
	float grayscale_color = 0.299*texture_color.r + 0.587*texture_color.g + 0.114*texture_color.b;
	color = (vec4(grayscale_color, grayscale_color, grayscale_color, 1.0) * grayScale) + (texture_color * (1.0f - grayScale));
	
	//	colorScale
	color = color * colorScale;

	//	offset color
	//color = color + offsetColor;
}                                                                              