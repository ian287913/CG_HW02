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