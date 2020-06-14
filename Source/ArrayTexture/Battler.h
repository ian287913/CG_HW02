#pragma once
#include "GameState.h"

const float runSpeedRatio = 0.1f;
const float KBRange = 1.0f;
const float KBHeight = 0.3f;
const float KBRatio = 0.25f;
enum BattlerState {idle, run, preAttack, knockback, die};

struct BattlerConfig
{
	float speed;
	float attackDelay;
	float KBHP;
	int lootMoney;
	BOConfig bof;
};

// 會自動移動, 攻擊的單位
class Battler : public BattleObject
{
public:
	static int allUnlootMoney;
	float speed;
	float attackDelay;
	int lootMoney;

	Battler(BattlerConfig config);
	~Battler();
	void Update(float deltaTime);
	float Damage(float amount);
protected:
	float KBingRange;
	float KBHP;
	float KBingHP;
	float attackTimer;
	float originalHeight;
	BattlerState state;
	void AI(float deltaTime);
	BattleObject* FindTarget();
	void Die() override;
};

////////////////////	.cpp
int Battler::allUnlootMoney = 0;

Battler::Battler(BattlerConfig config): BattleObject(config.bof)
{
	this->speed = config.speed;
	this->attackDelay = config.attackDelay;
	this->state = BattlerState::idle;
	this->KBHP = config.KBHP;
	this->KBingHP = 0;
	this->KBingRange = 0;
	this->attackTimer = 0;
	this->originalHeight = config.bof.height;
	this->lootMoney = config.lootMoney;
}
Battler::~Battler()
{
	// no need
}
void Battler::Update(float deltaTime)
{
	this->BattleObject::Update(deltaTime);
	if(!this->stopTime)
		this->AI(deltaTime);
}
float Battler::Damage(float amount)
{
	// cout << "Battler " << this->id << ": get damage: " << amount << endl;
	this->hp -= amount;
	ParticleSystem::CreateInstance(50, "Hit", vec2(0, 6.28f), vec2(-0.001f, -1.5f),
		vec2(0.0f, 0.2f), 0.2f, 0.2f,
		vec2(0, 6.28f), vec2(0.3f, 0.4f), 0.3, 1.0f);
	/*	parameters:
								  ( _amount, _spriteName, _directionLH, _speedLH,
		_spawnRadiusLH,	_fadeRadius, _fadeDistance,
		_rotationLH, _scaleLH, _lifetime, _timeSpeed)
	*/
	if (this->hp <= 0 && !isDying)
	{
		this->hp = 0;
		allUnlootMoney += this->lootMoney;
		this->Die();
	}

	// KB
	KBingHP += amount;
	if (KBingHP >= KBHP || this->hp <= 0)
	{
		KBingHP = 0;
		this->state = BattlerState::knockback;
		this->sprite->SetCurrentSet("hit");
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
				// cout << "Battler " << this->id << ": state run" << endl;
				this->state = BattlerState::run;
				this->sprite->SetCurrentSet("run");
			}
			else
			{
				//cout << "Battler " << this->id << ": state pre-attack" << endl;
				this->state = BattlerState::preAttack;
				this->attackTimer = attackDelay;
				this->sprite->SetCurrentSet("attack");
			}
		}
		break;
	case BattlerState::run:
	{
		BattleObject* target = FindTarget();
		if (target != NULL)
		{
			// cout << "Battler " << this->id << ": state preAttack" << endl;
			this->state = BattlerState::preAttack;
			this->attackTimer = attackDelay;
			this->sprite->SetCurrentSet("attack");
		}
	}
		break;
	case BattlerState::preAttack:
	{
		BattleObject* target = FindTarget();
		if (target == NULL)
		{
			// cout << "Battler " << this->id << ": state idle" << endl;
			this->state = BattlerState::idle;
			this->sprite->SetCurrentSet("idle");
		}
		else
		{
			if (this->attackTimer <= 0)
			{
				this->Attack(target);
				// cout << "Battler " << this->id << ": state idle" << endl;
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
				// cout << "Battler " << this->id << ": state run" << endl;
				this->state = BattlerState::run;
				this->sprite->SetCurrentSet("run");
				KBingRange = 0;
			}
			else
			{
				// cout << "Battler " << this->id << ": state die" << endl;
				this->state = BattlerState::die;
				sprite->SetCurrentSet("die");
				StartDestroy();
			}
			this->height = this->originalHeight;
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
		this->position += this->facing * deltaTime * speed * runSpeedRatio * 0.001f;
		// cout << "Battler " << this->id << ": pos: " << this->position << endl;
	}
	break;
	case BattlerState::preAttack:
	{
		if (this->attackTimer > 0)
			this->attackTimer -= deltaTime * 0.001f;
	}
	break;
	case BattlerState::knockback:
	{
		this->position -= this->facing * deltaTime * speed * runSpeedRatio * 1.5f * 0.001f;
		this->height = this->originalHeight + sinf(this->KBingRange / KBRange * 3.14159f);
		KBingRange += deltaTime * speed * runSpeedRatio * 1.5f * 0.001f;
	}
	break;
	case BattlerState::die:
	{
		// to die state
		// this->sprite->SetCurrentSet("die");
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
		if (this->facing * allObjects[i]->GameObject::facing >= 0)
			continue;
		float dist = abs(allObjects[i]->GameObject::position - this->position);
		// cout << "finding... dist: " << dist << endl;
		if (dist < this->attackRange && (closestDist < 0 || closestDist > dist))
		{
			closestDist = dist;
			target = allObjects[i];
		}
	}
	return target;
}

void Battler::Die()
{
	allObjects.erase(allObjects.begin() + this->id_bo);
	for (int i = allObjects.size() - 1; i >= 0; i--)
	{
		allObjects[i]->id_bo = i;
	}
}