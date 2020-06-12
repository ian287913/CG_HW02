#pragma once
#include "AnimationConfig.h"

const float totalFadeOutTime = 1.0f;

// 每個遊戲內的單位一定有包含圖組類型與現在的動畫state
class GameObject
{
public:
	static float timeScale;				// 所有人的時間縮放
	static bool stopTime;				// 所有人的時間是否靜止 (優先權高於縮放)
	static vector<GameObject*> actors;	// 此遊戲的所有單位
	static const float depthRatio;		// 深度與y軸的轉換比例
	int id;								// 在actors裡的id
	Animation* sprite;					// 角色的圖像資訊
	float position;						// x座標的位置, 左至右成長
	float height;						// y座標高度, 下至上成長
	float distance;						// 用於排列render的順序, 數字越高越早render, 也代表越容易被蓋住.
	float scale;						// 圖像的縮放
	float facing;						// 圖像的x軸縮放, 0代表面向左邊 1代表面向右邊
	float dyingTimer;					// 當 isDying 的時候會開始倒數, 為0時則destroy.
	float fading = 0.0f;				// shader fading 

	GameObject(string character, float pos, float high, float dist, float sizescale, bool facingRight, float dieTime);
	virtual ~GameObject();
	virtual void Update(float deltaTime);
	void StartDestroy();
protected:
	bool isDying;
	float fadeOutTimer;
	void rearangeID();
};

///////////////////////		.cpp

float GameObject::timeScale = 1.0f;
bool GameObject::stopTime = false;
vector<GameObject*> GameObject::actors = vector<GameObject*>();
const float GameObject::depthRatio = 0.5f;

void GameObject::Update(float deltaTime)
{
	this->sprite->Elapse(deltaTime);
	// 每幀call的function
	if (this->isDying)
	{
		if (this->dyingTimer < 0)
		{
			this->fadeOutTimer -= deltaTime * 0.001f;
			this->fading = 0.7f - this->fadeOutTimer / totalFadeOutTime;
			if(this->fadeOutTimer < -0.6f)
				delete this;
		}
		else
		{
			this->dyingTimer -= deltaTime * 0.001f;
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
	this->fading = 0.0f;
	this->fadeOutTimer = totalFadeOutTime;

	// cout << "GameObject " << this->id << ": constructing..." << endl;
	actors.push_back(this);
}
GameObject::~GameObject()
{
	// cout << "GameObject " << this->id << ": destructing..." << endl;
	actors.erase(actors.begin() + this->id);
	this->rearangeID();
	delete this->sprite;
	this->sprite = NULL;
}

void GameObject::StartDestroy()
{
	// cout << "GameObject " << this->id << ": Start destroy" << endl;
	isDying = true;
}

void GameObject::rearangeID()
{
	for (int i = actors.size() - 1; i >= 0; i--)
	{
		actors[i]->id = i;
	}
}