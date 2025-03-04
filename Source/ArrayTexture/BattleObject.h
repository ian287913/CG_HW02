#pragma once
#include"GameObject.h"
#include "ParticleSystem.h"

struct BOConfig
{
	float hp;
	float attack;
	float attackCD;
	float attackRange;

	string character;
	float pos;
	float height;
	float dist;
	float sizescale;
	bool facingRight;
	float dieTime;
};

class BattleObject : public GameObject
{
public:
	static vector<BattleObject*> allObjects;
	int id_bo;
	float hp;
	float attack;
	float attackCD;
	float attackRange;

	virtual float Damage(float amount);
	virtual void Attack(BattleObject* victim);
	virtual void Update(float deltaTime);
	BattleObject(BOConfig config);
	virtual ~BattleObject();
protected:
	float CDTimer;
	virtual void Die();
};

/////////////////	.cpp

vector<BattleObject*> BattleObject::allObjects = vector<BattleObject*>();

float BattleObject::Damage(float amount)
{
	// cout << "BattleObject " << this->id << ": get damage: " << amount << endl;
	this->hp -= amount;
	ParticleSystem::CreateInstance(vec3(this->position, this->height + this->distance * GameObject::depthRatio + 0.3f, 0),
		50, "Hit", vec2(0, 6.28f), vec2(-0.001f, -1.5f),
		vec2(0.0f, 0.2f), 0.2f, 0.2f,
		vec2(0, 6.28f), vec2(0.3f, 0.4f), 0.3, 1.0f);
	/*	parameters:
								  ( _amount, _spriteName, _directionLH, _speedLH,
		_spawnRadiusLH,	_fadeRadius, _fadeDistance,
		_rotationLH, _scaleLH, _lifetime, _timeSpeed)
	*/
	if (this->hp <= 0 && !isDying)
	{
		this->hp = 0;
		Die();
	}
	return this->hp;
}

void BattleObject::Attack(BattleObject* victim)
{
	// cout << "BattleObject " << this->id << ": attack BattleObject " << victim->id << endl;
	victim->Damage(this->attack);
	this->CDTimer = attackCD;
}

void BattleObject::Update(float deltaTime)
{
	this->GameObject::Update(deltaTime);
	if (CDTimer > 0 && !stopTime)
	{
		CDTimer -= deltaTime * timeScale * 0.001f;
	}
}

BattleObject::BattleObject(BOConfig config) : GameObject(config.character, config.pos, config.height, config.dist, config.sizescale, config.facingRight, config.dieTime)
{
	// cout << "BattleObject " << this->id << ": constructing..." << endl;
	this->hp = config.hp;
	this->attack = config.attack;
	this->attackCD = config.attackCD;
	this->attackRange = config.attackRange;
	this->CDTimer = 0;
	this->id_bo = allObjects.size();

	allObjects.push_back(this);
}
BattleObject::~BattleObject()
{
	// no need
	cout << "BattleObject " << this->id << ": destructing..." << endl;
}
void BattleObject::Die()
{
	// �����ʵe
	// cout << "BattleObject " << this->id << ": die." << endl;
	sprite->SetCurrentSet("die");
	allObjects.erase(allObjects.begin() + this->id_bo);
	this->id_bo = -1;
	for (int i = allObjects.size() - 1; i >= 0; i--)
	{
		allObjects[i]->id_bo = i;
	}
	StartDestroy();
}
