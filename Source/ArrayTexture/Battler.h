#pragma once
#include "BattleObject.h"

const float KBRange = 1.0f;
enum BattlerState {idle, run, attack, knockback, die};

// �|�۰ʲ���, ���������
class Tower : BattleObject
{
public:
	float speed;
	float attackSpeed;

	Tower();
	~Tower();
	void Update() override;
	void Die() override;
	void Attack(BattleObject* victim) override;
protected:
	BattlerState state;
};