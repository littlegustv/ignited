#pragma once

#include "GameObject.h"

class Camera : public GameObject
{
private:
	ALLEGRO_TRANSFORM trans;
public:
	Camera();
	void Destroy();
	void Update(int x, int y);
	void Render();
	void Collided();
};