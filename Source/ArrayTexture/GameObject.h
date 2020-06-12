#pragma once
#include "AnimationConfig.h"

// �C�ӹC���������@�w���]�t�ϲ������P�{�b���ʵestate
class GameObject
{
public:
	static float timeScale;				// �Ҧ��H���ɶ��Y��
	static bool stopTime;				// �Ҧ��H���ɶ��O�_�R�� (�u���v�����Y��)
	static vector<GameObject*> actors;	// ���C�����Ҧ����
	static const float depthRatio;		// �`�׻Py�b���ഫ���
	int id;								// �bactors�̪�id
	Animation* sprite;					// ���⪺�Ϲ���T
	float position;						// x�y�Ъ���m, ���ܥk����
	float height;						// y�y�а���, �U�ܤW����
	float distance;						// �Ω�ƦCrender������, �Ʀr�V���V��render, �]�N��V�e���Q�\��.
	float scale;						// �Ϲ����Y��
	float facing;						// �Ϲ���x�b�Y��, 0�N���V���� 1�N���V�k��
	float dyingTimer;					// �� isDying ���ɭԷ|�}�l�˼�, ��0�ɫhdestroy.

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
const float GameObject::depthRatio = 0.5f;

void GameObject::Update(float deltaTime)
{
	this->sprite->Elapse(deltaTime);
	// �C�Vcall��function
	if (isDying)
	{
		dyingTimer -= deltaTime * 0.001f;
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

	cout << "GameObject " << this->id << ": constructing..." << endl;
	actors.push_back(this);
}
GameObject::~GameObject()
{
	cout << "GameObject " << this->id << ": destructing..." << endl;
	actors.erase(actors.begin() + this->id);
	delete this->sprite;
	this->sprite = NULL;
}

void GameObject::StartDestroy()
{
	cout << "GameObject " << this->id << ": Start destroy" << endl;
	isDying = true;
}