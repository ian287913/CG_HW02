#include"GameObject.h"

float GameObject::timeScale = 1.0f;
bool GameObject::stopTime = false;

void GameObject::Update()
{
	// ¨C´Vcall Animationªºupdate()
}

GameObject::GameObject()
{

}
GameObject::~GameObject()
{
	delete sprite;
	sprite = NULL;
}