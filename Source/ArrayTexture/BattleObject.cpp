#include"BattleObject.h"

vector<BattleObject*> BattleObject::allObjects = vector<BattleObject*>();

float BattleObject::Damage(float amount)
{
	this->hp -= amount;
	if (amount <= 0 && !isDying)
	{
		this->hp = 0;
		Die();
	}
	return this->hp;
}

void BattleObject::Attack(BattleObject* victim)
{
	victim->Damage(this->attack);
	this->CDTimer = attackCD;
}

void BattleObject::Update(float deltaTime)
{
	this->GameObject::Update(deltaTime);
	if (CDTimer > 0 && !stopTime)
	{
		CDTimer -= deltaTime * timeScale;
	}
}

BattleObject::BattleObject(BOConfig config) : GameObject(config.character, config.pos, config.height, config.dist, config.sizescale, config.facingRight, config.dieTime)
{
	this->hp = config.hp;
	this->attack = config.attack;
	this->attackCD = config.attackCD;
	this->attackRange = config.attackRange;
	this->CDTimer = 0;
}
BattleObject::~BattleObject()
{
	// no need
}
void BattleObject::Die()
{
	// ¤Á´«°Êµe
	sprite->SetCurrentSet("die");
	allObjects.erase(allObjects.begin() + this->id_bo);
	StartDestroy();
}