#pragma once
#include <cmath>
#include "../../Include/Common.h"
using namespace glm;

#define DOR(angle) (angle*3.1415/180);
mat4 translate(float x, float y, float z) {
	vec4 t = vec4(x, y, z, 1);//w = 1 ,�hx,y,z=0�ɤ]��translate
	vec4 c1 = vec4(1, 0, 0, 0);
	vec4 c2 = vec4(0, 1, 0, 0);
	vec4 c3 = vec4(0, 0, 1, 0);
	mat4 M = mat4(c1, c2, c3, t);
	return M;
}
mat4 scale(float x, float y, float z) {
	vec4 c1 = vec4(x, 0, 0, 0);
	vec4 c2 = vec4(0, y, 0, 0);
	vec4 c3 = vec4(0, 0, z, 0);
	vec4 c4 = vec4(0, 0, 0, 1);
	mat4 M = mat4(c1, c2, c3, c4);
	return M;
}
mat4 rotate(float angle, float x, float y, float z) {
	float r = DOR(angle);
	mat4 M = mat4(1);

	vec4 c1 = vec4(cos(r) + (1 - cos(r))*x*x, (1 - cos(r))*y*x + sin(r)*z, (1 - cos(r))*z*x - sin(r)*y, 0);
	vec4 c2 = vec4((1 - cos(r))*y*x - sin(r)*z, cos(r) + (1 - cos(r))*y*y, (1 - cos(r))*z*y + sin(r)*x, 0);
	vec4 c3 = vec4((1 - cos(r))*z*x + sin(r)*y, (1 - cos(r))*z*y - sin(r)*x, cos(r) + (1 - cos(r))*z*z, 0);
	vec4 c4 = vec4(0, 0, 0, 1);
	M = mat4(c1, c2, c3, c4);
	return M;
}

//	random helper
float RandomFloat(float lo, float hi)
{
	return linearRand(lo, hi);
}
glm::vec2 RandomPosition(float xRange, float yRange)
{
	float r = RandomFloat(1.0f, 1000.0f);
	vec2 pos = diskRand(r);
	pos.x *= xRange / r;
	pos.y *= yRange / r;

	return pos;
}

float LengthOfVec2(glm::vec2 &_vec2)
{
	return sqrtf(_vec2.x * _vec2.x + _vec2.y * _vec2.y);
}

void NormalizeVec2(glm::vec2 &_vec2)
{
	if (LengthOfVec2(_vec2) != 0)
		_vec2 /= LengthOfVec2(_vec2);
}

glm::vec2 RandomPointRadius(float minRadius, float maxRadius)
{
	vec2 direction = vec2(RandomFloat(-1.0f, 1.0f), RandomFloat(-1.0f, 1.0f));
	NormalizeVec2(direction);
	return direction * RandomFloat(minRadius, maxRadius);
}



