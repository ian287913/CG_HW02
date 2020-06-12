#pragma once
#include "BattleObject.h"

// ������, �Q�ʧ���, �|�u�|���˪����
class Tower : public BattleObject
{
public:
	bool isEnemy;
	bool gameOver = false;
	Tower(BOConfig config, bool enemy);
	~Tower();
	// �I�s�ӧQ�Υ��ѵe��
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
	// �b���I�s�ӧQ�Υ���
}