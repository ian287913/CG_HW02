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
	void Die() override;
};

//////////////////	.cpp

Tower::Tower(BOConfig config, bool enemy) : BattleObject(config)
{
	cout << "Tower " << this->id << ": constructing..." << endl;
	this->isEnemy = enemy;
}
Tower::~Tower()
{
	cout << "Tower " << this->id << ": destructing..." << endl;
}
void Tower::Die()
{
	cout << "Tower " << this->id << ": Die." << endl;
	sprite->SetCurrentSet("die");
	// 在此呼叫勝利或失敗
}