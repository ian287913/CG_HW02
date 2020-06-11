#pragma once
#include "AnimationConfig.h"

// 每個遊戲內的單位一定有包含圖組類型與現在的動畫state
class GameObject
{
public:
	static float timeScale;				// 所有人的時間縮放
	static bool stopTime;				// 所有人的時間是否靜止 (優先權高於縮放)
	static vector<GameObject*> actors;	// 此遊戲的所有單位
	int id;								// 在actors裡的id
	Animation* sprite;					// 角色的圖像資訊
	float position;						// x座標的位置, 左至右成長
	float height;						// y座標高度, 下至上成長
	float distance;						// 用於排列render的順序, 數字越高越早render, 也代表越容易被蓋住.
	float scale;						// 圖像的縮放
	float facing;						// 圖像的x軸縮放, 0代表面向左邊 1代表面向右邊
	float dyingTimer;					// 當 isDying 的時候會開始倒數, 為0時則destroy.

	GameObject(string character, float pos, float high, float dist, float sizescale, bool facingRight, float dieTime);
	virtual ~GameObject();
	virtual void Update(float deltaTime);
	void StartDestroy();
protected:
	bool isDying;
};

///////////////////////		.cpp

float GameObject::timeScale = 1.0f;
bool GameObject::stopTime = false;
vector<GameObject*> GameObject::actors = vector<GameObject*>();

void GameObject::Update(float deltaTime)
{
	// 每幀call的function
	if (isDying)
	{
		dyingTimer -= deltaTime;
		if (dyingTimer < 0)
		{
			delete this;
		}
	}
}

GameObject::GameObject(string character, float pos, float high, float dist, float sizescale, bool facingRight, float dieTime)
{
	this->sprite = new Animation(character);
	this->position = pos;
	this->height = high;
	this->distance = dist;
	this->scale = sizescale;
	this->facing = facingRight ? 1 : -1;
	this->dyingTimer = dieTime;
	this->isDying = false;
	this->id = actors.size();

	actors.push_back(this);
}
GameObject::~GameObject()
{

	actors.erase(actors.begin() + this->id);
	delete this->sprite;
	this->sprite = NULL;
}

void GameObject::StartDestroy()
{
	isDying = true;
}