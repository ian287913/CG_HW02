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
	// �b���I�s�ӧQ�Υ���
}