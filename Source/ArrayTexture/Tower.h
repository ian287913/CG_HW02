#pragma once
#include "BattleObject.h"

const float KBRange = 1.0f;
enum BattlerState {idle, run, attack, knockback, die};

// 不移動, 被動攻擊, 會只會受傷的單位
class Tower : BattleObject
{
public:
	Tower();
	~Tower();
	// 
	void Update() override;
	// 呼叫勝利或失敗畫面
	void Die() override;
	// 雷射砲
	void Attack(BattleObject* victim) override;
};