#pragma once
#include "BattleObject.h"

// ������, �Q�ʧ���, �|�u�|���˪����
class Tower : BattleObject
{
public:
	bool isEnemy;
	Tower(BOConfig config, bool enemy);
	~Tower();
	// �I�s�ӧQ�Υ��ѵe��
	void Die();
};

//////////////////	.cpp

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