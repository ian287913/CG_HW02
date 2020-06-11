#pragma once
#include "BattleObject.h"

const float KBRange = 1.0f;
enum BattlerState {idle, run, attack, knockback, die};

// ������, �Q�ʧ���, �|�u�|���˪����
class Tower : BattleObject
{
public:
	Tower();
	~Tower();
	// 
	void Update() override;
	// �I�s�ӧQ�Υ��ѵe��
	void Die() override;
	// �p�g��
	void Attack(BattleObject* victim) override;
};