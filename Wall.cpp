#include "Wall.h"

Wall::Wall(int x, int y, ALLEGRO_BITMAP *image)
{
	GameObject::Init(x, y, 0, 0, 16, 16);
	Wall::image = image;

	SetID(SOLID, 0);

	frameHeight = 64;
	frameWidth = 32;
	z = 0;
}

void Wall::Destroy()
{
	GameObject::Destroy();
}

void Wall::Render()
{
	//GameObject::Render();

	if (debug)
		al_draw_rectangle(x, y, x + 2*boundX, y + 2*boundY, al_map_rgb(255, 200, 100), 2);

	//al_draw_bitmap(image, x, y - frameHeight / 2, 0);
}
void Wall::Update()
{
	//GameObject::Update();
}

void Wall::Collided(GameObject *other)
{}