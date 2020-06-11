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