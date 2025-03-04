#pragma once
#include "BattleObject.h"

// 不移動, 被動攻擊, 會只會受傷的單位
class Tower : public BattleObject
{
public:
	bool isEnemy;
	bool gameOver = false;
	Tower(BOConfig config, bool enemy);
	~Tower();
	// 呼叫勝利或失敗畫面
	void Die() override;
};

//////////////////	.cpp

Tower::Tower(BOConfig config, bool enemy) : BattleObject(config)
{
	cout << "Tower " << this->id << ": constructing..." << endl;
	gameOver = false;
	this->isEnemy = enemy;
}
Tower::~Tower()
{
	cout << "Tower " << this->id << ": destructing..." << endl;
}
void Tower::Die()
{
	if (gameOver)
		return;
	cout << "Tower " << this->id << ": Die." << endl;
	sprite->SetCurrentSet("die");
	gameOver = true;
	// 在此呼叫勝利或失敗
}