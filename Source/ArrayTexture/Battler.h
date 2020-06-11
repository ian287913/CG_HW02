#pragma once
#include "BattleObject.h"

const float KBRange = 1.0f;
enum BattlerState {idle, run, attack, knockback, die};

// 會自動移動, 攻擊的單位
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