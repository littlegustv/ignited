#pragma once

#include "GameObject.h"

class Effect : public GameObject
{
private:
	int frames;
public:
	Effect(int x, int y, int velX, int velY, int frames, std::string imageFile);
	void Destroy();
	void Update();
	void Render();
	void Collided(GameObject *other);
};