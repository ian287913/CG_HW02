#include"Tower.h"

Tower::Tower(BOConfig config, bool enemy) : BattleObject(config)
{
	this->isEnemy = enemy;
}
Tower::~Tower()
{
}
void Tower::Die()
{
	sprite->SetCurrentSet("die");
	// 在此呼叫勝利或失敗
}