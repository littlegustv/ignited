#pragma once

#include "GameObject.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

class Background : public GameObject
{
private:
	int fade = 0;
	float scroll = 0;

public:
	Background(std::string data, ALLEGRO_BITMAP *backbuffer, ALLEGRO_BITMAP *tilesets[10], int tilesetInfo[10][2], int z, int w, int h, int fade = 0, int scroll = 0);
	Background(int z, int x, int y, std::string imageFile);
	void Destroy();
	void Update();
	void Render();
	void Collided();
};