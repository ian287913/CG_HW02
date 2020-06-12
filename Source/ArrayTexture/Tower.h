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
	// �b���I�s�ӧQ�Υ���
}