#pragma once
#include "AnimationConfig.h"

// 每個遊戲內的單位一定有包含圖組類型與現在的動畫state
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