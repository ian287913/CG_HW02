#pragma once
#include "AnimationConfig.h"

// �C�ӹC���������@�w���]�t�ϲ������P�{�b���ʵestate
class GameObject
{
public:
	static float timeScale;
	static bool stopTime;
	Animation* sprite;
	float position;
	float distance;
	float scale;

	GameObject();
	~GameObject();
	virtual void Update();
};