#include "Hazard.h"

Hazard::Hazard(int x, int y, std::string imageFile)
{
	GameObject::Init(x, y, 0, 0, 16, 16);
	SetID(HAZARD, 0);
//	SetID(SOLID, 1);

	maxFrame = 4;
	curFrame = 0;
	frameCount = 0;
	frameDelay = 12;
	animationColumns = 4;

	frameHeight = 48;
	frameWidth = 32;
	z = 1;

	animationDirection = 1;

	Hazard::imageFile = imageFile;

	Hazard::image = al_load_bitmap(imageFile.c_str());
}
void Hazard::Destroy()
{
	GameObject::Destroy();
	
}
void Hazard::Update()
{
	GameObject::Update();

	if (++frameCount > frameDelay)
	{
		curFrame += animationDirection;
		if (curFrame >= maxFrame)
			curFrame = 0;
		else if (curFrame <= 0)
			curFrame = maxFrame;

		frameCount = 0;
		a = -1 * (a - 1);
	}

}
void Hazard::Render()
{
	int fx = (curFrame % animationColumns) * frameWidth;
	int fy = facing * frameHeight;
	
	al_draw_bitmap_region(image, fx, fy, frameWidth, frameHeight, x, y - 16, 0);
//	al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_ONE);
//	al_draw_tinted_bitmap_region(image, al_map_rgba_f(a, a, a, a), fx, fy, frameWidth, frameHeight, x, y - 16, 0);
//	al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_INVERSE_ALPHA);
//	al_draw_rectangle(x, y, x + 2 * boundX, y + 2 * boundY, al_map_rgb(100, 200, 140), 2);
}
void Hazard::Collided()
{

}