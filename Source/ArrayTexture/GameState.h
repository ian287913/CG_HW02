#pragma once
#include "Tower.h"
#include "Battler.h"
#define CHARNUM 5
#define ENMYNUM 3
#define LVLNUM 5

struct CharacterConfig
{
	string name;
	BattlerConfig config;
};

// 玩家使用的角色
const vector<CharacterConfig> characterTable
{
	// name, speed, delay, kb, hp, att, attCD, range, sprite, x, y, z, size, enemy, diedelay
	{"L_Tank", {10, 0.8f, 50, {250, 20, 1.0f, 1.5f, "L_Tank", 0, 0, 0, 2.5, true, 3}}},
	{"L_Ranger", {10, 2.0f, 50, {250, 30, 1.0f, 4.0f, "L_Ranger", 0, 0, 0, 3.0f, true, 3}}}
};

// 左方AI使用的角色
const vector<CharacterConfig> characterTable_enemy
{
	// name, speed, delay, kb, hp, att, attCD, range, sprite, x, y, z, size, enemy, diedelay
	{"L_Tank", {10, 0.8f, 50, {250, 20, 1.0f, 1.5f, "L_Tank", 0, 0, 0, 2.5, true, 3}}},
	{"L_Ranger", {10, 2.0f, 50, {250, 30, 1.0f, 4.0f, "L_Ranger", 0, 0, 0, 3.0f, true, 3}}},
	{"L_Tank", {10, 0.8f, 50, {250, 20, 1.0f, 1.5f, "L_Tank", 0, 0, 0, 2.5, true, 3}}}
};

static class GameState
{
public:
	static const float spawnCD[CHARNUM];
	static const float laserCD;
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
	static const int enemySellectWeight[ENMYNUM];
	
	int currentMoney;
	int currentLevel;
	Tower* leftTower;
	Tower* rightTower;
	bool GameOver;
	bool enableEnemyAI;

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
	float spawnCDing[CHARNUM];
	float laserCDing;
	float AICDing;
	int allWeights;
	bool constructed = false;
	// update呼叫的敵方AI
	void EnemyAI(float deltaTime);
};

// 參數定義
const float GameState::spawnCD[CHARNUM] = {2, 2, 2, 2.5f, 5.0f};
const float GameState::laserCD = 10;
const float GameState::AICD = 8;
const float GameState::AICD_range = 1.5f;
const int GameState::cost[CHARNUM] = {75, 150, 300, 600, 1000};
const int GameState::maxMoney_level[LVLNUM] = {500, 1000, 1500, 2000, 2500};
const int GameState::lvUP_cost[LVLNUM] = {250, 750, 1200, 1750, 3000};
const int GameState::rateMoney_level[LVLNUM] = {50, 100, 150, 200, 250};
const float GameState::leftSpawnPos = -7;
const float GameState::rightSpawnPos = 7;
const float GameState::spawnDistance = -5;
const float GameState::spawnDistanceRange = 0.1f;
const float GameState::towerHP = 1000;
const float GameState::towerAttack = 10000;
const int GameState::enemySellectWeight[ENMYNUM] = {3, 1, 0};

// .cpp

void GameState::Laser()
{
	cout << "Game: Use laser!"<< endl;
	//特效與攻擊
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
	cout << "Game: Add Battler of " << table[index].name << endl;
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

	// 每個 gameObject
	for (int i = GameObject::actors.size() - 1; i >= 0; i--)
	{
		GameObject::actors[i]->Update(deltaTime);
	}

	// 金錢增加
	currentMoney += floorl(rateMoney_level[currentLevel] * deltaTime);
	if (currentMoney > maxMoney_level[currentLevel])
		currentMoney = maxMoney_level[currentLevel];

	// CDing
	for (int i = 0; i < CHARNUM; i++)
	{
		spawnCDing[i] -= deltaTime;
		if (spawnCDing[i] < 0)
		{
			spawnCDing[i] = 0;
		}
	}
	laserCDing -= deltaTime;
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
			GameOver = true;
		}
		else if(rightTower->hp <= 0)
		{
			// 勝利
			cout << "----------------AI Wins!!----------------" << endl;
			GameOver = true;
		}
	}

	EnemyAI(deltaTime);
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
	laserCDing = 0;
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
		BOConfig config = { towerHP, towerAttack, laserCD, 10, "L_Tower", leftSpawnPos, 0, spawnDistance, 5, true, 100 };
		leftTower = new Tower(config, true);
	}
	// 造塔 (右)
	{
		// HP, attack, attackCD, attackrange {string character; pos; height; dist; sizescale; facingRight; dieTime; }
		BOConfig config = { towerHP, towerAttack, laserCD, 10, "L_Tower", rightSpawnPos, 0, spawnDistance, 5, false, 100 };
		leftTower = new Tower(config, false);
	}
	constructed = true;
}

GameState::~GameState()
{
	cout << "Game: destructing..." << endl;
	for (int i = GameObject::actors.size() - 1; i >= 0; i--)
	{
		GameObject::actors[i]->StartDestroy();
	}
}