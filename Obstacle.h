#pragma once

#include "GameObject.h"

class Obstacle : public GameObject
{
private:
	int pushed = -1;
	bool canMove = false;
public:
	Obstacle(int x, int y, std::string imageFile, bool canMove);
	void Destroy();
	void Update();
	void Render();
	void Collided(GameObject *other);
	void SetPushed(int direction);
};