#pragma once

#include "GameObject.h"

class Exit : public GameObject
{
private:
	int toLevel;
	int toX;
	int toY;
	void(*GoToLevel)(int l, int x, int y, bool p);
	float showMessage = 0.0;
	std::string message = "";
	bool locked = false;
	bool play = true;

public:
	Exit(int x, int y, int toLevel, void(*gotolevel)(int, int, int, bool), int playerX, int playerY, bool locked, bool play = true);
	void Destroy();
	void Render();
	void Update();
	void Collided(GameObject *other);
	bool IsLocked() { return locked; }
	void SetLocked(bool locked){ Exit::locked = locked; }
	bool PlaySound() { return play; }
};