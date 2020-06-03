#pragma once
#include <string>
#include <vector>
#include "../../Include/Common.h"

using namespace std;

struct AnimFrameSet
{
	string ownerName;

	string setName;
	vector<pair<int, int>> rawFrames;
	vector<int> frames;
	float FPS;
	bool loop;
	string nextSet;
};

struct AnimationConfig	//	spritesheet
{
	string name;

	string imgPath;
	int rowCount;
	int colCount;
	vector<AnimFrameSet> frameSets;
};

class Animation
{
	Sprite2D* spriteSheet;
	AnimFrameSet *currentSet;	//	current set
	vector<AnimFrameSet> frameSets;
	int frame;	//	current frame

	float extraTime = 0;
	float currentTime, spritePlayTime = 0;

	void Elapse(float deltaTime);
	AnimFrameSet* FindFrameSet(string setName);
	void StepFrame();
};
void Animation::Elapse(float deltaTime)
{
	extraTime += deltaTime;
	//	next frame
	if (extraTime > (1000.0f / currentSet->FPS))
	{
		extraTime -= (1000.0f / currentSet->FPS);
		///frame = currentSet->frames.size()
		///...
	}

	/*currentTime = glutGet(GLUT_ELAPSED_TIME);
	spritePlayTime += (currentTime - lastTime) / 1000.0f;
	spriteIndex = (int)(spritePlayTime * sprite->GetFPS() + startFrame) % sprite->GetCount();*/
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
void Animation::StepFrame()
{

}


vector<AnimationConfig> AnimationConfigs{
	{ "L_Tank", "/Lionar/f1_tank.png" , 12, 6},
	{ "L_Ranger", "/Lionar/f1_tank.png" , 12, 6}
};

vector<AnimFrameSet> AnimFrameSets{
	{ "L_Tank", "attack", { {2, 3}, {4, 6}, {4, 5}}, {}, 3, false },
	{ "L_Tank", "walk", { {0, 1}, {3, 4}, {6, 6}}, {}, 3, false },

};
