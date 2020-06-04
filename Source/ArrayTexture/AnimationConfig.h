#pragma once
#include <string>
#include <vector>
#include "../../Include/Common.h"

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
	vector<AnimFrameSet> frameSets;
};

//	these are predefined animation datas
vector<AnimationConfig> AnimationConfigTable{
	{ "L_Tank", "/Lionar/f1_tank.png" , 12, 6},
};
vector<AnimFrameSet> AnimFrameSetTable{
	{ "L_Tank", "run", { {0, 7}, {0, 6}, {0, 5}, {0, 4}, {0, 3}, {0, 2}, {0, 1}, {0, 0}}, {}, 3, true },
	{ "L_Tank", "hit", { {1, 8}, {1, 7}, {1, 8}, {1, 7}, {1, 8}}, {}, 3, true },

};

class Animation
{
	Sprite2D* spriteSheet = NULL;
	AnimFrameSet *currentSet = NULL;	//	current set
	vector<AnimFrameSet> frameSets;
	int frame;	//	current frame
	float extraTime = 0;
	///float currentTime, spritePlayTime = 0;

public:
	Animation();
	Animation(AnimationConfig config, string imagePath);

	void Elapse(float deltaTime);
	AnimFrameSet* FindFrameSet(string setName);
	void StepFrame();
	void SetCurrentSet(string setName);
	int GetSheetFrame();
	//	renderer usage
	bool Enable();
	bool Disable();
};
Animation::Animation()
{

}
Animation::Animation(AnimationConfig config, string imagePath)
{
	//	create sprite
	cout << "LoadCharacterSpritesheet: " << config.imgPath << "\n";
	spriteSheet = new Sprite2D();
	spriteSheet->Init(imagePath + config.imgPath, config.rowCount, config.colCount, 3, false, 0, 0);

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
			frameSets.push_back(set);
		}
	}

	if (frameSets.size() > 0)
		currentSet = &(frameSets[0]);
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
	for (int i = 0; i < frameSets.size(); i++)
	{
		if (frameSets[i].setName == setName)
		{
			return &(frameSets[i]);
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
		else
		{

		}
	}
	else
	{
		frame++;
	}
}
void Animation::SetCurrentSet(string setName)
{
	AnimFrameSet* nextSet = NULL;
	if ((nextSet = FindFrameSet(currentSet->nextSet)) != NULL)
	{
		currentSet = nextSet;
		frame = 0;
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




