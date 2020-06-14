#pragma once
#include <iomanip>
#include "Tower.h"
#include "Battler.h"
#define CHARNUM 5
#define ENMYNUM 3
#define LVLNUM 5

void GameOverTitle(bool win);

struct CharacterConfig
{
	string name;
	BattlerConfig config;
};

// 玩家使用的角色
const vector<CharacterConfig> characterTable
{
	// name, speed, delay, kb, loot, hp, att, attCD, range, sprite, x, y, z, size, enemy, diedelay
	{"L_Tank", {10, 0.8f, 50, 50, {250, 20, 1.0f, 1.5f, "L_Tank", 0, 0, 0, 2.5, true, 1.5f}}},
	{"L_Ranger", {10, 2.0f, 50, 70, {250, 30, 1.0f, 4.0f, "L_Ranger", 0, 0, 0, 3.0f, true, 1.5f}}}
};

// 左方AI使用的角色
const vector<CharacterConfig> characterTable_enemy
{
	// name, speed, delay, kb, loot, hp, att, attCD, range, sprite, x, y, z, size, enemy, diedelay
	{"A_Minion", {10, 0.9f, 50, 50, {90, 15, 1.0f, 1.5f, "A_Minion", 0, 0, 0, 1, true, 1.5f}}},
	{"A_Ranger", {15, 1.5f, 40, 70, {250, 40, 1.3f, 4.0f, "A_Ranger", 0, 0, 0, 1, true, 1.5f}}},
	{"A_Shield", {10, 1.6f, 100, 100, {1000, 50, 1.7f, 1.5f, "A_Shield", 0, 0, 0, 1, true, 1.5f}}}
};

static class GameState
{
public:
	static const float spawnCD[CHARNUM];
	static const float laserCD;
	static const float laserDelay;
	static const float AICD;
	static const float AICD_range;
	static const int cost[CHARNUM];
	static const int maxMoney_level[LVLNUM];
	static const int rateMoney_level[LVLNUM];
	static const int lvUP_cost[LVLNUM];
	static const float leftSpawnPos;
	static const float rightSpawnPos;
	static const float spawnDistance;
	static const float spawnDistanceRange;
	static const float towerHP;
	static const float towerAttack;
	static const float laserRange;
	static const float laserSpeed;
	static const float laserFireOffset;
	static const int enemySellectWeight[ENMYNUM];
	
	int currentMoney;
	int currentLevel;
	Tower* leftTower;
	Tower* rightTower;
	bool GameOver;
	bool enableEnemyAI;
	float spawnCDing[CHARNUM];
	float laserCDing;

	bool CanLevelUp();
	bool CanAdd(int index);
	void AddMoney(int amount);
	// 開雷射砲
	void Laser();
	// 升級金錢
	void LevelUp();
	// 新增單一兵種
	Battler* AddBattler(string name, bool isEnemy = false);
	Battler* AddBattler(int index, bool isEnemy);
	// 更新所有的GameObject
	void Update(float deltaTime);
	GameState();
	~GameState();
protected:
	float AICDing;
	float laserTimer;
	vector<BattleObject*> laseredObjects;
	float laseringRange;
	float effectingRange;
	int allWeights;
	bool constructed = false;
	// update呼叫的敵方AI
	void EnemyAI(float deltaTime);
	void KillAll(bool toEnemy);
	void LaserAttack();
};

// 參數定義
const float GameState::spawnCD[CHARNUM] = {2, 2, 2, 2.5f, 5.0f};
const float GameState::laserCD = 20;
const float GameState::laserDelay = 2.0f;
const float GameState::AICD = 10;
const float GameState::AICD_range = 2.5f;
const int GameState::cost[CHARNUM] = {75, 150, 300, 600, 1000};
const int GameState::maxMoney_level[LVLNUM] = {500, 1000, 1500, 2000, 2500};
const int GameState::lvUP_cost[LVLNUM] = {250, 750, 1200, 1750, 3000};
const int GameState::rateMoney_level[LVLNUM] = {50, 60, 75, 90, 110};
const float GameState::leftSpawnPos = -7;
const float GameState::rightSpawnPos = 7;
const float GameState::spawnDistance = -4.5;
const float GameState::spawnDistanceRange = 0.6f;
const float GameState::towerHP = 1000;
const float GameState::towerAttack = 1000;
const float GameState::laserRange = 15;
const float GameState::laserSpeed = 60;
const float GameState::laserFireOffset = 1.2f;
const int GameState::enemySellectWeight[ENMYNUM] = {4, 1, 1};

// .cpp

bool GameState::CanLevelUp()
{
	return ((currentLevel < LVLNUM - 1) && (currentMoney >= lvUP_cost[currentLevel]));
}

bool GameState::CanAdd(int index)
{
	return ((currentMoney >= cost[index]) && (spawnCDing[index] <= 0));
}

void GameState::AddMoney(int amount)
{
	currentMoney += amount;
	if (currentMoney > maxMoney_level[currentLevel])
		currentMoney = maxMoney_level[currentLevel];
}

void GameState::Laser()
{
	if (laserCDing > 0)
	{
		cout << "Game: Can't use laser! CD: " << laserCDing << " second(s)." << endl;
		return;
	}
	cout << "Game: Use laser!" << endl;
	// n秒後攻擊
	laserTimer = laserDelay;
	laseredObjects.clear();
	// 進cd
	laserCDing = laserCD;
}
void GameState::LaserAttack()
{
	vector<BattleObject*> addLaser = vector<BattleObject*>();
	for (int i = BattleObject::allObjects.size() - 1; i >= 0; i--)
	{
		if (BattleObject::allObjects[i]->facing > 0 && (BattleObject::allObjects[i]->id_bo != leftTower->id_bo))
		{
			if (BattleObject::allObjects[i]->position < rightSpawnPos - laseringRange)
				continue;

			bool inLasered = false;
			for (int j = laseredObjects.size() - 1; j >= 0; j--)
			{
				if (laseredObjects[j] == NULL)
					continue;
				if (laseredObjects[j]->id_bo == BattleObject::allObjects[i]->id_bo)
				{
					inLasered = true;
					break;
				}
			}
			if (!inLasered)
			{
				addLaser.push_back(BattleObject::allObjects[i]);
			}
		}
	}
	for (int i = 0; i < addLaser.size(); i++)
	{
		laseredObjects.push_back(addLaser[i]);
		addLaser[i]->Damage(towerAttack);
	}
}

void GameState::LevelUp()
{
	if (currentLevel < LVLNUM - 1)
	{
		if (currentMoney >= lvUP_cost[currentLevel])
		{
			cout << "Game: Level Up! from lv." << currentLevel << " to lv." << currentLevel + 1 << endl;
			currentMoney -= lvUP_cost[currentLevel];
			currentLevel++;
		}
		else
		{
			cout << "Game: Can't level up! You don't have enough money! Require: " << lvUP_cost[currentLevel] << endl;
		}
	}
	else
	{
		cout << "Game: Can't level up! You're in max Lv." << currentLevel << endl;
	}
}

Battler* GameState::AddBattler(string name, bool isEnemy)
{
	for (int i = 0; i < characterTable.size(); i++)
	{
		if (characterTable[i].name == name)
		{
			return AddBattler(i, false);
		}
	}
	return NULL;
}

Battler* GameState::AddBattler(int index, bool isEnemy)
{
	vector<CharacterConfig> table =
		isEnemy ? characterTable_enemy : characterTable;
	if (index >= table.size())
	{
		return NULL;
	}
	if (!isEnemy)
	{
		if (currentMoney < cost[index])
		{
			cout << "Game: Can't add battler, you don't have enough money! Current money: " << currentMoney << endl;
			return NULL;
		}
		if (spawnCDing[index] > 0)
		{
			cout << "Game: Can't add battler, this type of battler is in cooldown: " << std::setprecision(2) << spawnCDing[index] << " second(s)." << endl;
			return NULL;
		}
		currentMoney -= cost[index];
		spawnCDing[index] = spawnCD[index];
		cout << "Game: Add Battler of " << table[index].name << ", cost: "  << cost[index] << endl;
	}
	else
	{
		cout << "Game: Add enemy " << table[index].name << endl;
	}
	BattlerConfig config = table[index].config;
	config.bof.pos = isEnemy ? leftSpawnPos : rightSpawnPos;
	config.bof.dist = spawnDistance + (spawnDistanceRange * 2) * rand() / (RAND_MAX + 1.0) - spawnDistanceRange;
	config.bof.facingRight = isEnemy;
	return new Battler(config);
}

void GameState::Update(float deltaTime)
{
	if (!constructed)
		return;

	// 每個 gameObject update
	for (int i = GameObject::actors.size() - 1; i >= 0; i--)
	{
		GameObject::actors[i]->Update(deltaTime);
	}

	// 金錢增加
	if (Battler::allUnlootMoney > 0)
	{
		cout << "Game: Looted money: +" << Battler::allUnlootMoney << endl;
		AddMoney(Battler::allUnlootMoney);
		Battler::allUnlootMoney = 0;
	}
	AddMoney(floorl(rateMoney_level[currentLevel] * deltaTime * 0.001f));

	// CDing
	for (int i = 0; i < CHARNUM; i++)
	{
		spawnCDing[i] -= deltaTime * 0.001f;
		if (spawnCDing[i] < 0)
		{
			spawnCDing[i] = 0;
		}
	}
	laserCDing -= deltaTime * 0.001f;
	if (laserCDing < 0)
	{
		laserCDing = 0;
	}

	// 檢查勝敗
	if (!GameOver && (leftTower != NULL && rightTower != NULL))
	{
		if (leftTower->hp <= 0)
		{
			// 勝利
			cout << "----------------Player Wins!!----------------" << endl;
			KillAll(true);
			GameOver = true;
			GameOverTitle(true);
		}
		else if(rightTower->hp <= 0)
		{
			// 勝利
			cout << "----------------AI Wins!!----------------" << endl;
			KillAll(false);
			GameOver = true;
			GameOverTitle(false);
		}
	}

	// enemy AI
	EnemyAI(deltaTime);

	// laser
	if (laserTimer > 0)
	{
		laserTimer -= deltaTime * 0.001f;
		if (laserTimer <= 0)
		{
			laseringRange = 0;
		}
	}
	if (laseringRange < laserRange)
	{
		laseringRange += laserSpeed * deltaTime * 0.001f;
		LaserAttack();
	}
}

// 自動隨機出兵
void GameState::EnemyAI(float deltaTime)
{
	if (!enableEnemyAI || GameOver)
		return;
	if (AICDing > 0)
		AICDing -= deltaTime * 0.001f;
	else
	{
		int randomIndex = rand() % allWeights;
		for (int i = 0; i < ENMYNUM; i++)
		{
			randomIndex -= enemySellectWeight[i];
			if (randomIndex < 0)
			{
				AddBattler(i, true);
				break;
			}
		}
		AICDing = AICD + (AICD_range * 2) * rand() / (RAND_MAX + 1.0) - AICD_range;
	}
	
	
}

void GameState::KillAll(bool toEnemy)
{
	for (int i = BattleObject::allObjects.size() - 1; i >= 0; i--)
	{
		if (toEnemy && BattleObject::allObjects[i]->facing > 0)
		{
			BattleObject::allObjects[i]->Damage(BattleObject::allObjects[i]->hp);
		}
		else if (!toEnemy && BattleObject::allObjects[i]->facing < 0)
			BattleObject::allObjects[i]->Damage(BattleObject::allObjects[i]->hp);
	}
}

GameState::GameState()
{
	constructed = false;
	cout << "Game: constructing..." << endl;
	// 變數初始化
	currentMoney = 0;
	currentLevel = 0;
	GameOver = false;
	enableEnemyAI = true;
	for (int i = 0; i < CHARNUM; i++)
	{
		spawnCDing[i] = 0;
	}
	laserCDing = laserCD;
	laserTimer = -1;
	laseringRange = laserRange;
	laseredObjects = vector<BattleObject*>();
	AICDing = AICD;
	allWeights = 0;
	for (int i = 0; i < ENMYNUM; i++)
	{
		allWeights += enemySellectWeight[i];
	}
	leftTower = NULL;
	rightTower = NULL;

	// 造塔 (左)
	{
		// HP, attack, attackCD, attackrange {string character; pos; height; dist; sizescale; facingRight; dieTime; }
		BOConfig config = { towerHP, towerAttack, laserCD, 10, "A_Tower", leftSpawnPos, 0, spawnDistance, 1, true, 100 };
		leftTower = new Tower(config, true);
	}
	// 造塔 (右)
	{
		// HP, attack, attackCD, attackrange {string character; pos; height; dist; sizescale; facingRight; dieTime; }
		BOConfig config = { towerHP, towerAttack, laserCD, 10, "L_Tower", rightSpawnPos, 0, spawnDistance, 5, false, 100 };
		rightTower = new Tower(config, false);
	}
	constructed = true;
}

GameState::~GameState()
{
	cout << "Game: destructing..." << endl;
	for (int i = GameObject::actors.size() - 1; i >= 0; i--)
	{
		delete GameObject::actors[i];
	}
	GameObject::actors = vector<GameObject*>();
	BattleObject::allObjects = vector<BattleObject*>();
}