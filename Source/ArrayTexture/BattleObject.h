#pragma once
#include"GameObject.h"

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

class BattleObject : protected GameObject
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
	this->hp -= amount;
	if (amount <= 0 && !isDying)
	{
		this->hp = 0;
		Die();
	}
	return this->hp;
}

void BattleObject::Attack(BattleObject* victim)
{
	victim->Damage(this->attack);
	this->CDTimer = attackCD;
}

void BattleObject::Update(float deltaTime)
{
	this->GameObject::Update(deltaTime);
	if (CDTimer > 0 && !stopTime)
	{
		CDTimer -= deltaTime * timeScale;
	}
}

BattleObject::BattleObject(BOConfig config) : GameObject(config.character, config.pos, config.height, config.dist, config.sizescale, config.facingRight, config.dieTime)
{
	this->hp = config.hp;
	this->attack = config.attack;
	this->attackCD = config.attackCD;
	this->attackRange = config.attackRange;
	this->CDTimer = 0;
}
BattleObject::~BattleObject()
{
	// no need
}
void BattleObject::Die()
{
	// ¤Á´«°Êµe
	sprite->SetCurrentSet("die");
	allObjects.erase(allObjects.begin() + this->id_bo);
	StartDestroy();
}
