#pragma once

#include "GameObject.h"

class Wall : public GameObject
{
private:
public:
	Wall(int x, int y, ALLEGRO_BITMAP *image);

	void Destroy();

	void Render();
	void Update();

	void Collided(GameObject *other);
};