#include"GameObject.h"

float GameObject::timeScale = 1.0f;
bool GameObject::stopTime = false;

void GameObject::Update()
{
	// �C�Vcall Animation��update()
}

GameObject::GameObject()
{

}
GameObject::~GameObject()
{
	delete sprite;
	sprite = NULL;
}