#pragma once
#include <string>
#include <vector>
#include "../../Include/Common.h"
#include "TranslateRoteteScaleHelper.h"

using namespace std;

struct AnimFrameSet
{
	string ownerName;	//	used to match AnimationConfig

	string setName;
	vector<pair<int, int>> rawFrames;
	vector<int> frames;
	float FPS;
	bool loop;
	string nextSet;
};
//	spritesheet
struct AnimationConfig
{
	string name;

	string imgPath;
	int rowCount;
	int colCount;
	float anchorOffsetX;
	float anchorOffsetY;
	float anchorScale;
	float shadowOffsetX;
	float shadowOffsetY;
	float shadowScale;
	vector<AnimFrameSet> frameSets;
};

//	these are predefined animation datas
/*
Minion
Tank
Ranged
Lancer	(spd, kb)
Super	(high damage & health, kb-resist)

*/

vector<AnimationConfig> AnimationConfigTable{
	//{ "L_Basic", "/Lionar/f1_tank.png" , 12, 6},
	{ "L_Minion"	, "/Lionar/f1_silverguardsquire.png", 12, 4		, 0.0f, 0.25f, 2.0f		, 0.02f, -0.33f, 0.56f},
	{ "L_Tank"	, "/Lionar/f1_tank.png"					, 12, 6		, 0.0f, 0.25f, 1.0f		, 0.02f, -0.3f, 0.56f},
	{ "L_Ranger", "/Lionar/f1_backlinearcher.png"		, 8, 16		, 0.0f, 0.35f, 1.0f		, 0.04f, -0.33f, 0.45f},
	{ "L_Lancer"	, "/Lionar/f1_radiantdragoon.png"	, 10, 8		, 0.0f, 0.32f, 3.4f		, -0.02f, -0.34f, 0.26f},
	{ "L_Tower"	, "/Lionar/f1_ironcliffemonument.png"	, 10, 10	, 0.0f, 0.3f, 1.0f		, 0.0f, -0.33f, 0.56f},
	{ "A_Tower"	, "/Abyssian/f4_buildlegendary.png"		, 10, 10	, 0.0f, 0.3f, 5.0f		, 0.0f, -0.32f, 0.4f},
	{ "A_Minion"	, "/Abyssian/neutral_shadow1.png"	, 12, 4		, 0.0f, 0.27f, 2.8f		, 0.0f, -0.28f, 0.3f},
	{ "A_Ranger"	, "/Abyssian/boss_skurge.png"		, 12, 5		, 0.0f, 0.27f, 3.0f		, 0.0f, -0.28f, 0.3f},
	{ "A_Shield"	, "/Abyssian/boss_manaman.png"		, 10, 10	, 0.0f, 0.42f, 3.0f		, 0.0f, -0.43f, 0.3f},
	
	
	
	{ "Icon_Ultimate"		, "/Icon/icon_f1_tempest.png"	, 5, 5		, 0.0f, 0.0f, 1.0f		, 0.0f, 0.0f, 0.0f},
	{ "FX_BeamFire"			, "/FX/fx_beamfire.png"				, 5, 7		, 0.0f, 0.4f, 9.0f		, 0.0f, 0.0f, 0.0f},
	{ "FX_BeamLaser"		, "/FX/fx_beamlaser.png"			, 2, 17		, 0.0f, 0.4f, 7.0f		, 0.0f, 0.0f, 0.0f},
	{ "FX_BeamLaser_Fixed"	, "/FX/fx_beamlaser_fixed.png"		, 2, 17		, 0.0f, 0.154f, 7.0f		, 0.0f, 0.0f, 0.0f},
};

vector<AnimFrameSet> AnimFrameSetTable{
	{ "L_Minion", "idle",		{	{1, 11}, {1, 4},  }, {}, 2, true },
	{ "L_Minion", "run",		{	{0, 7}, {0, 6}, {0, 5}, {0, 4}, {0, 3}, {0, 2}, {0, 1}, {0, 0}}, {}, 6, true },
	{ "L_Minion", "attack",		{	{1, 11}, {3, 9}, {3, 8}, {3, 7}, {3, 6}, {3, 5}, {3, 4}, {3, 3}, {3, 2}, {3, 1}, {3, 0}, {2, 11}, {2, 10},  }, {}, 12, false, "idle" },
	{ "L_Minion", "hit",		{	{1, 11}, {1, 6}, {1, 5},  }, {}, 3, false, "idle" },
	{ "L_Minion", "die",		{	{1, 11}, {2, 4}, {2, 3}, {2, 2}, {2, 1}, {2, 0}, {3, 10}, {1, 10}, {1, 9}, {1, 8}, {1, 7},  }, {}, 6, false },


	{ "L_Tank", "idle",		{	{2, 3}, {1, 5}, {1, 4}, {1, 3}, {1, 2}, {1, 1}, {1, 0}, {0, 11}, {0, 10}, {0, 9}, {0, 8},  }, {}, 6, true },
	{ "L_Tank", "run",		{	{0, 7}, {0, 6}, {0, 5}, {0, 4}, {0, 3}, {0, 2}, {0, 1}, {0, 0}}, {}, 6, true },
	{ "L_Tank", "hit",		{	{1, 8}, {1, 7}, {1, 8},  }, {}, 1, false, "idle" },
	//{ "L_Tank", "hit",		{	{1, 8}, {1, 7}, {1, 8}, {1, 7}, {1, 8},  }, {}, 6, false, "idle" },
	{ "L_Tank", "attack",	{	{2, 3}, {4, 6}, {4, 5}, {4, 4}, {4, 3}, {4, 2}, {4, 1}, {4, 0}, {3, 11}, {3, 10}, 
								{3, 9}, {3, 8}, {3, 7}, {3, 6}, {3, 5}, {3, 4}, {3, 3}, {3, 2}, {2, 3},  }, {}, 12, false, "idle" },
	{ "L_Tank", "die",		{	{2, 8}, {2, 7}, {2, 6}, {2, 5}, {2, 4}, {4, 7}, {2, 2}, {2, 1}, {2, 0}, {1, 11}, {1, 10}, {1, 9},  }, {}, 6, false },


	{ "L_Ranger", "idle",		{	{1, 5}, {1, 4}, {1, 3}, {1, 2}, {1, 1}, {1, 0}, {0, 7}, {0, 6}, {0, 5}, {0, 4}, {0, 3}}, {}, 6, true },
	{ "L_Ranger", "run",		{	{4, 1}, {8, 1}, {8, 0}, {7, 7}, {7, 6}, {7, 5}, {7, 4}, {7, 3}}, {}, 9, true },
	{ "L_Ranger", "hit",		{	{3, 5}, {3, 4}, {3, 2}}, {}, 6, true },
	{ "L_Ranger", "attack",		{	{7, 2}, {7, 1}, {7, 0}, {6, 7}, {6, 6}, {6, 5}, {6, 4}, {6, 3}, {6, 2}, {6, 1}, {6, 0}, {5, 7}, {5, 6}, {5, 5}, {5, 4}, {5, 3}, {5, 2}, {5, 1}, {5, 0}, {4, 7}, {4, 6}, {4, 5}, {4, 4}, {4, 3}}, {}, 10, false, "idle" },
	{ "L_Ranger", "die",		{	{3, 5}, {3, 4}, {3, 3}, {3, 2}, {3, 1}, {3, 0}, {2, 7}, {2, 6}, {2, 5}, {2, 4}, {2, 3}, {2, 2}, {2, 1}, {2, 0}, {1, 7}}, {}, 6, false },

	//...
	{ "L_Lancer", "idle",		{	{1, 9}, {1, 8}, {1, 7}, {1, 6}, {1, 5}, {1, 4}, {1, 3}, {1, 2}, {1, 1}, {1, 0}, {0, 9}, {0, 8},  }, {}, 6, true },
	{ "L_Lancer", "run",		{	{0, 7}, {0, 6}, {0, 5}, {0, 4}, {0, 3}, {0, 2}, {0, 1}, {0, 0}}, {}, 6, true },
	{ "L_Lancer", "attack",		{	{3, 5}, {7, 0}, {6, 9}, {6, 8}, {6, 7}, {6, 6}, {6, 5}, {6, 4}, {6, 3}, {6, 2}, {6, 1}, {6, 0}, {5, 9}, {5, 8}, {5, 7}, {5, 6}, {5, 5}, {5, 4}, {5, 3}, {5, 2}, {5, 1}, {5, 0}, {4, 9}, {4, 8}, {4, 7},  }, {}, 12, false, "idle" },
	{ "L_Lancer", "hit",		{	{3, 6}, {7, 1}, {2, 0},  }, {}, 3, false, "idle" },
	{ "L_Lancer", "die",		{	{3, 6}, {7, 1}, {3, 4}, {3, 3}, {3, 2}, {3, 1}, {3, 0}, {2, 9}, {2, 8}, {2, 7}, {2, 6}, {2, 5}, {2, 4}, {2, 3}, {2, 2}, {2, 1},  }, {}, 6, false },



	{ "L_Tower", "idle",		{	{3, 3}, {1, 4}, {1, 3}, {1, 2}, {1, 1}, {1, 0}, {0, 9}, {0, 8}, {0, 7}, {0, 6}, {0, 5}, {0, 4}, {0, 3}, {0, 2}, {0, 1}, {0, 0}}, {}, 6, true },
	{ "L_Tower", "attack",		{	{2, 6}, {5, 2}, {5, 1}, {5, 0}, {4, 9}, {4, 8}, {4, 7}, {4, 6}, {4, 5}, {4, 4}, {4, 3}, {4, 2}, {4, 1}, {4, 0}, {3, 9}, {3, 8}, {3, 7}, {3, 6}, {3, 5}, {3, 4}}, {}, 6, false, "idle" },
	{ "L_Tower", "hit",			{	{2, 5}, {2, 4}, {2, 3}}, {}, 6, false, "idle" },
	{ "L_Tower", "die",			{	{2, 5}, {2, 4}, {2, 3}, {2, 2}, {2, 1}, {2, 0}, {1, 9}, {1, 8}, {1, 7}, {1, 6}, {1, 5}}, {}, 6, false },
	
	{ "A_Tower", "idle",		{	{3, 0}, {1, 2}, {1, 1}, {1, 0}, {0, 9}, {0, 8}, {0, 7}, {0, 6}, {0, 5}, {0, 4}, {0, 3}, {0, 2}, {0, 1}, {0, 0}}, {}, 6, true },
	{ "A_Tower", "hit",			{	{3, 1}, {6, 1}, {2, 9}}, {}, 6, false, "idle" },
	{ "A_Tower", "die",			{	{3, 1}, {6, 1}, {2, 9}, {2, 8}, {2, 7}, {2, 6}, {2, 5}, {2, 4}, {2, 3}, {2, 2}, {2, 1}, {2, 0}, {1, 9}, {1, 8}, {1, 7}, {1, 6}, {1, 5}, {1, 4}, {1, 3}}, {}, 6, false },

	{ "A_Minion", "idle",		{	{2, 11}, {1, 0}, {0, 11}, {0, 10}, {0, 9}, {0, 8}, {0, 7}, {0, 6},  }, {}, 6, true },
	{ "A_Minion", "run",		{	{0, 5}, {0, 4}, {0, 3}, {0, 2}, {0, 1}, {0, 0},  }, {}, 6, true },
	{ "A_Minion", "attack",		{	{1, 11}, {3, 9}, {3, 8}, {3, 7}, {3, 6}, {3, 5}, {3, 4}, {3, 3}, {3, 2}, {3, 1}, {3, 0},  }, {}, 6, false, "idle" },
	{ "A_Minion", "die",		{	{3, 10}, {1, 10}, {1, 9}, {1, 8}, {1, 7}, {1, 6}, {1, 5}, {1, 4},  }, {}, 6, false},
	{ "A_Minion", "hit",		{	{1, 3}, {1, 2}, {1, 1},  }, {}, 6, false, "idle"},

	{ "A_Ranger", "idle",		{	{1, 10}, {1, 9}, {1, 8}, {1, 7}, {1, 6}, {1, 5}, {1, 4}, {1, 3}, {1, 2}, {1, 1}, {1, 0},  }, {}, 6, true },
	{ "A_Ranger", "run",		{	{0, 7}, {0, 6}, {0, 5}, {0, 4}, {0, 3}, {0, 2}, {0, 1}, {0, 0},  }, {}, 6, true },
	{ "A_Ranger", "attack",		{	{2, 2}, {4, 3}, {4, 2}, {4, 1}, {4, 0}, {3, 11}, {3, 10}, {3, 9}, {3, 8}, {3, 7}, {3, 6}, {3, 5}, {3, 4}, {3, 3},  }, {}, 6, false, "idle" },
	{ "A_Ranger", "die",		{	{2, 10}, {2, 9}, {2, 8}, {2, 7}, {2, 6}, {2, 5}, {2, 4}, {2, 3}, {4, 4},  }, {}, 6, false},
	{ "A_Ranger", "hit",		{	{2, 1}, {2, 0}, {1, 11},  }, {}, 6, false, "idle"},

	{ "A_Shield", "idle",		{	{1, 3}, {1, 2}, {1, 1}, {1, 0}, {0, 9}, {0, 8}, {0, 7}, {0, 6},  }, {}, 6, true },
	{ "A_Shield", "run",		{	{0, 5}, {0, 4}, {0, 3}, {0, 2}, {0, 1}, {0, 0},  }, {}, 6, true },
	{ "A_Shield", "attack",		{	{4, 5}, {9, 0}, {8, 9}, {8, 8}, {8, 7}, {8, 6}, {8, 5}, {8, 4}, {8, 3}, {8, 2}, {8, 1}, {8, 0}, {7, 9}, {7, 8}, {7, 7}, {7, 6}, {7, 5}, {7, 4}, {7, 3}, {7, 2}, {7, 1}, {7, 0}, {6, 6}, {6, 5}, {6, 4}, {6, 3}, {6, 2}, {6, 1}, {6, 0}, {5, 9}, {5, 8}, {5, 7},  }, {}, 10, false, "idle" },
	{ "A_Shield", "die",		{	{4, 4}, {4, 3}, {4, 2}, {4, 1}, {4, 0}, {3, 9}, {3, 8}, {3, 7}, {3, 6}, {3, 5}, {3, 4}, {3, 3}, {3, 2}, {3, 1}, {3, 0}, {2, 9}, {2, 8}, {2, 7}, {2, 6}, {2, 5}, {2, 4}, {2, 3}, {2, 2}, {2, 1}, {2, 0}, {1, 9}, {1, 8},  }, {}, 6, false},
	{ "A_Shield", "hit",		{	{5, 6}, {1, 7}, {1, 6}, {1, 5}, {1, 4},  }, {}, 6, false, "idle"},


	{ "Icon_Ultimate", "idle",			{	{1, 3}, {3, 1}, {3, 0}, {2, 4}, {2, 3}, {2, 2}}, {}, 3, true },
	{ "Icon_Ultimate", "run",			{	{2, 1}, {2, 0}, {1, 4}, {3, 2}, {1, 2}, {1, 1}, {1, 0}, {0, 4}, {0, 3}, {0, 2}, {0, 1}, {0, 0}}, {}, 6, true },

	{ "FX_BeamFire", "idle",				{	{4, 3}, {4, 2}, {4, 1}, {4, 0}, {3, 4}, {3, 3}, {3, 2}, {3, 1}, {6, 0}, {2, 4}, {2, 3}, {2, 2}, {2, 1}, {2, 0}, {1, 4}, {1, 3}, {1, 2}, {1, 1}, {1, 0}, {0, 4}, {0, 3}, {0, 2}, {0, 1}, {0, 0}}, {}, 10, false },
	{ "FX_BeamFire", "run",			{	{3, 0}, {5, 4}, {5, 3}, {5, 2}, {5, 1}, {5, 0}, {4, 4}, {4, 3}, {4, 2}, {4, 1}, {4, 0}, {3, 4}, {3, 3}, {3, 2}, {3, 1}, {6, 0}, {2, 4}, {2, 3}, {2, 2}, {2, 1}, {2, 0}, {1, 4}, {1, 3}, {1, 2}, {1, 1}, {1, 0}, {0, 4}, {0, 3}, {0, 2}, {0, 1}, {0, 0}}, {}, 2, true },

	{ "FX_BeamLaser", "idle",			{	{8, 0}, {15, 1}, {15, 0}, {14, 1}, {14, 0}, {13, 1}, {13, 0}, {12, 1}, {12, 0}, {11, 1}, {11, 0}, {10, 1}, {10, 0}, {9, 1}, {9, 0}, {8, 1}, {16, 0}, {7, 1}, {7, 0}, {6, 1}, {6, 0}, {5, 1}, {5, 0}, {4, 1}, {4, 0}, {3, 1}, {3, 0}, {2, 1}, {2, 0}, {1, 1}, {1, 0}, {0, 1}, {0, 0}}, {}, 6, true },

	{ "FX_BeamLaser_Fixed", "idle",		{	{5, 0}, {4, 1}, {4, 0}, {4, 1}, {5, 0}, {4, 1}, {4, 0}, {3, 1}, {3, 0}, {2, 1}, {2, 0}, {1, 1}, {1, 0}, {0, 1}, {0, 0}}, {}, 10, false },
	{ "FX_BeamLaser_Fixed", "run",			{	{8, 0}, {15, 1}, {15, 0}, {14, 1}, {14, 0}, {13, 1}, {13, 0}, {12, 1}, {12, 0}, {11, 1}, {11, 0}, {10, 1}, {10, 0}, {9, 1}, {9, 0}, {8, 1}, {16, 0}, {7, 1}, {7, 0}, {6, 1}, {6, 0}, {5, 1}, {5, 0}, {4, 1}, {4, 0}, {3, 1}, {3, 0}, {2, 1}, {2, 0}, {1, 1}, {1, 0}, {0, 1}, {0, 0}}, {}, 6, true },

};

struct SpriteAndFramesets
{
	string name;
	Sprite2D* spriteSheet;
	vector<AnimFrameSet> frameSets;
	float anchorOffsetX;
	float anchorOffsetY;
	float anchorScale;
	float shadowOffsetX;
	float shadowOffsetY;
	float shadowScale;
};

class Animation
{
public:
	static vector<SpriteAndFramesets*> SpriteAndFramesetsList;

	Sprite2D *spriteSheet = NULL;		//	referenced sprite (texture)
	vector<AnimFrameSet> *frameSets;	//	referenced FrameSets;

	AnimFrameSet *currentSet;			//	current set
	int frame;							//	current frame
	mat4 anchorTranslate;
	float shadowOffsetX;
	float shadowOffsetY;
	float shadowScale;
	float extraTime = 0;

public:
	static void InitializeAllSpritesAndSets(string imagePath);

	Animation();
	Animation(string characterName);

	void Elapse(float deltaTime);
	AnimFrameSet* FindFrameSet(string setName);
	void StepFrame();
	void SetCurrentSet(string setName);
	int GetSheetFrame();
	//	renderer usage
	bool Enable();
	bool Disable();
};
vector<SpriteAndFramesets*> Animation::SpriteAndFramesetsList;
void Animation::InitializeAllSpritesAndSets(string imagePath)
{
	for each (AnimationConfig config in AnimationConfigTable)
	{
		SpriteAndFramesets* newAnimationData = new SpriteAndFramesets();
		newAnimationData->name = config.name;

		//	create sprite
		cout << "LoadCharacterSpritesheet: " << config.imgPath << "\n";
		newAnimationData->spriteSheet = new Sprite2D();
		newAnimationData->spriteSheet->Init(imagePath + config.imgPath, config.rowCount, config.colCount, 3, false, 0, 0);

		//	get all sets
		for each (AnimFrameSet set in AnimFrameSetTable)
		{
			if (set.ownerName == config.name)
			{
				//	digest rawFrames to frames
				set.frames.clear();
				for each (pair<int, int> framePair in set.rawFrames)
				{
					set.frames.push_back(framePair.first * config.rowCount + framePair.second);
				}

				//	add this set
				newAnimationData->frameSets.push_back(set);
			}
		}

		if (newAnimationData->frameSets.size() <= 0)
		{
			cout << "###\t frameSets.size() <= 0   : " << config.name << ".\N";

		}

		newAnimationData->anchorOffsetX = config.anchorOffsetX;
		newAnimationData->anchorOffsetY = config.anchorOffsetY;
		newAnimationData->anchorScale = config.anchorScale;

		newAnimationData->shadowOffsetX = config.shadowOffsetX;
		newAnimationData->shadowOffsetY = config.shadowOffsetY;
		newAnimationData->shadowScale = config.shadowScale;

		SpriteAndFramesetsList.push_back(newAnimationData);
	}
	
}

Animation::Animation()
{

}
Animation::Animation(string characterName)
{
	SpriteAndFramesets *foundSpriteAndFramesets = NULL;
	//	get SpriteAndFramesets
	for each (SpriteAndFramesets *spriteAndFramesets in SpriteAndFramesetsList)
	{
		if (spriteAndFramesets->name == characterName)
		{
			foundSpriteAndFramesets = spriteAndFramesets;
			break;
		}
	}
	if (foundSpriteAndFramesets == NULL)
	{
		cout << "###\tCannot find config of characterName: " << characterName << ".\N";
		return;
	}

	//	reference sprite
	spriteSheet = foundSpriteAndFramesets->spriteSheet;
	frameSets = &(foundSpriteAndFramesets->frameSets);
	//	set anchor offset
	anchorTranslate = scale(foundSpriteAndFramesets->anchorScale, foundSpriteAndFramesets->anchorScale, 1.0f) * translate(foundSpriteAndFramesets->anchorOffsetX, foundSpriteAndFramesets->anchorOffsetY, 0);
	//	set shadow offsets
	shadowOffsetX = foundSpriteAndFramesets->shadowOffsetX;
	shadowOffsetY = foundSpriteAndFramesets->shadowOffsetY;
	shadowScale = foundSpriteAndFramesets->shadowScale;
	
	if (frameSets->size() > 0)
		currentSet = &((*frameSets)[0]);
	else
	{
		cout << "##\t frameSets.size <= 0.  " << characterName << "\n";
	}
}

void Animation::Elapse(float deltaTime)
{
	extraTime += deltaTime;
	//	next frame
	if (extraTime > (1000.0f / currentSet->FPS))
	{
		extraTime -= (1000.0f / currentSet->FPS);
		StepFrame();
	}
}
AnimFrameSet* Animation::FindFrameSet(string setName)
{
	for (int i = 0; i < (*frameSets).size(); i++)
	{
		if ((*frameSets)[i].setName == setName)
		{
			return &((*frameSets)[i]);
		}
	}
	return NULL;
}
//	goto next frame. loop or next handle included
void Animation::StepFrame()
{
	AnimFrameSet* nextSet = NULL;

	//	end of current set
	if (frame >= (currentSet->frames.size() - 1))
	{
		//	loop
		if (currentSet->loop)
		{
			frame = 0;
		}
		//	go to next set
		else if ((nextSet = FindFrameSet(currentSet->nextSet)) != NULL)
		{
			currentSet = nextSet;
			frame = 0;
		}
		//	no loop and no next set. just stay at current frame...
		else {}
	}
	else
	{
		frame++;
	}
}
void Animation::SetCurrentSet(string setName)
{
	AnimFrameSet* nextSet = NULL;
	if ((nextSet = FindFrameSet(setName)) != NULL)
	{
		currentSet = nextSet;
		frame = 0;
	}
	else
	{
		cout << "## set AnimState failed. " << setName << " state NOT FOUND.\n";
	}
}
int Animation::GetSheetFrame()
{
	if (spriteSheet != NULL && currentSet != NULL)
	{
		return currentSet->frames[frame];
	}
	return 0;
}
bool Animation::Enable()
{
	if (currentSet == NULL || spriteSheet == NULL)
		return false;
	else
		spriteSheet->Enable();
	return true;
}
bool Animation::Disable()
{
	if (spriteSheet == NULL)
		return false;
	else
		spriteSheet->Disable();
	return true;
}




