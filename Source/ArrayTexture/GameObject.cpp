#include"GameObject.h"

float GameObject::timeScale = 1.0f;
bool GameObject::stopTime = false;
vector<GameObject*> GameObject::actors = vector<GameObject*>();

void GameObject::Update(float deltaTime)
{
	// ¨C´Vcallªºfunction
	if (isDying)
	{
		dyingTimer -= deltaTime;
		if (dyingTimer < 0)
		{
			delete this;
		}
	}
}

GameObject::GameObject(string character, float pos, float high, float dist, float sizescale, bool facingRight, float dieTime)
{
	this->sprite = new Animation(character);
	this->position = pos;
	this->height = high;
	this->distance = dist;
	this->scale = sizescale;
	this->facing = facingRight ? 1 : -1;
	this->dyingTimer = dieTime;
	this->isDying = false;
	this->id = actors.size();

	actors.push_back(this);
}
GameObject::~GameObject()
{
	
	actors.erase(actors.begin() + this->id);
	delete this->sprite;
	this->sprite = NULL;
}

void GameObject::StartDestroy()
{
	isDying = true;
}