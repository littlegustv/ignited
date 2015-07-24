#pragma once

#include "GameObject.h"
#include "Level.h"

class Level;

class Hazard : public GameObject
{
private:
	bool spawned = false;
	int a = 0;
public:
	Hazard(int x, int y, std::string imageFile);
	void Destroy();
	void Update();
	void Render();
	void Collided();
	bool CanSpawn() { return !spawned; }
	void SetSpawned(bool spawned) { Hazard::spawned = spawned; }

};