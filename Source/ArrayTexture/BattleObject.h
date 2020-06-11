#pragma once
#include"GameObject.h"
class BattleObject : GameObject
{
public:
	float hp;
	float attack;
	float attackCD;
	float size;

	float Damage(float amount);
	virtual void Attack(BattleObject* victim);
	BattleObject();
	~BattleObject();
protected:
	float CDTimer;
	virtual void Die();
};