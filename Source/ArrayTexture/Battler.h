#pragma once
#include "BattleObject.h"

const float KBRange = 1.0f;
const float KBRatio = 0.25f;
enum BattlerState {idle, run, preAttack, knockback, die};

struct BattlerConfig
{
	float speed;
	float attackDelay;
	float KBHP;
	BOConfig bof;
};

// 會自動移動, 攻擊的單位
class Battler : BattleObject
{
public:
	float speed;
	float attackDelay;

	Battler(BattlerConfig config);
	~Battler();
	void Update(float deltaTime);
	float Damage(float amount);
protected:
	float KBingRange;
	float KBHP;
	float KBingHP;
	float attackTimer;
	BattlerState state;
	void AI(float deltaTime);
	BattleObject* FindTarget();
};

////////////////////	.cpp

Battler::Battler(BattlerConfig config): BattleObject(config.bof)
{
	cout << "Battler " << this->id << ": constructing..." << endl;
	this->speed = config.speed;
	this->attackDelay = config.attackDelay;
	this->state = BattlerState::idle;
	this->KBHP = config.KBHP;
	this->KBingHP = 0;
	this->KBingRange = 0;
	this->attackTimer = 0;
}
Battler::~Battler()
{
	// no need
	cout << "Battler " << this->id << ": destructing..." << endl;
}
void Battler::Update(float deltaTime)
{
	this->BattleObject::Update(deltaTime);
	if(!this->stopTime)
		this->AI(deltaTime);
}
float Battler::Damage(float amount)
{
	cout << "Battler " << this->id << ": get damage: " << amount << endl;
	this->hp -= amount;
	if (amount <= 0 && !isDying)
	{
		this->hp = 0;
	}

	// KB
	KBingHP += amount;
	if (KBingHP >= KBHP || this->hp <= 0)
	{
		KBingHP = 0;
		this->state = BattlerState::knockback;
		this->KBingRange = 0;
	}
	
	return this->hp;

}

void Battler::AI(float deltaTime)
{
	// 確認狀態
	switch (this->state)
	{
	case BattlerState::idle:
		if (CDTimer <= 0)
		{
			BattleObject* target = FindTarget();
			if (target == NULL)
			{
				cout << "Battler " << this->id << ": state run" << endl;
				this->state = BattlerState::run;
			}
			else
			{
				cout << "Battler " << this->id << ": state pre-attack" << endl;
				this->state = BattlerState::preAttack;
				this->attackTimer = attackDelay;
			}
		}
		break;
	case BattlerState::run:
	{
		BattleObject* target = FindTarget();
		if (target != NULL)
		{
			cout << "Battler " << this->id << ": state preAttack" << endl;
			this->state = BattlerState::preAttack;
		}
	}
		break;
	case BattlerState::preAttack:
	{
		BattleObject* target = FindTarget();
		if (target == NULL)
		{
			cout << "Battler " << this->id << ": state idle" << endl;
			this->state = BattlerState::idle;
		}
		else
		{
			if (this->attackTimer <= 0)
			{
				this->Attack(target);
				cout << "Battler " << this->id << ": state idle" << endl;
				this->state = BattlerState::idle;
				this->CDTimer = this->attackCD;
			}
		}
	}
		break;
	case BattlerState::knockback:
	{
		if (KBingRange >= KBRange)
		{
			if (this->hp > 0)
			{
				cout << "Battler " << this->id << ": state run" << endl;
				this->state = BattlerState::run;
				KBingRange = 0;
			}
			else
			{
				cout << "Battler " << this->id << ": state die" << endl;
				this->state = BattlerState::die;
				this->Die();
			}
		}
	}
		break;
	case BattlerState::die:
	{
		// checks nothing
	}
	break;
	default:
		break;
	}

	// 執行狀態
	switch (this->state)
	{
	case BattlerState::idle:
	{
		// does nothing
	}
		break;
	case BattlerState::run:
	{
		this->position += this->facing * deltaTime * speed;
	}
	break;
	case BattlerState::preAttack:
	{
		if (this->attackTimer > 0)
			this->attackTimer -= deltaTime;
	}
	break;
	case BattlerState::knockback:
	{
		this->position -= this->facing * deltaTime * speed;
		KBingRange += deltaTime * speed;
	}
	break;
	case BattlerState::die:
	{
		// to die state
		this->sprite->SetCurrentSet("die");
	}
	break;
	default:
		break;
	}
}

BattleObject* Battler::FindTarget()
{
	BattleObject* target = NULL;
	float closestDist = -1;
	for (int i = allObjects.size() - 1; i >= 0; i--)
	{
		// 特別不檢查每個人的hp是否為0, 因為tower hp為0 還是照打
		if (allObjects[i]->GameObject::id == this->id || this->facing * allObjects[i]->GameObject::facing >= 0)
			continue;
		float dist = abs(allObjects[i]->GameObject::position - this->position);
		if (dist < this->attackRange && (closestDist < 0 || closestDist > dist))
		{
			closestDist = dist;
			target = allObjects[i];
		}
	}
	return target;
}