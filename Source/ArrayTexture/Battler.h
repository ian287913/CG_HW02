#pragma once
#include "BattleObject.h"

const float KBRange = 1.0f;
const float KBRatio = 0.25f;
enum BattlerState {idle, run, preAttack, knockback, die};

struct BattlerConfig
{
	float speed;
	float attackDelay;
	float KBHP;
	BOConfig bof;
};

// 會自動移動, 攻擊的單位
class Battler : BattleObject
{
public:
	float speed;
	float attackDelay;

	Battler(BattlerConfig config);
	~Battler();
	void Update(float deltaTime);
	float Damage(float amount);
protected:
	float KBingRange;
	float KBHP;
	float KBingHP;
	float attackTimer;
	BattlerState state;
	void AI(float deltaTime);
	BattleObject* FindTarget();
};