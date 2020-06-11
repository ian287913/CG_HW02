#pragma once
#include "BattleObject.h"

// 不移動, 被動攻擊, 會只會受傷的單位
class Tower : BattleObject
{
public:
	bool isEnemy;
	Tower(BOConfig config, bool enemy);
	~Tower();
	// 呼叫勝利或失敗畫面
	void Die();
};