#include "Effect.h"

Effect::Effect(int x, int y, int velX, int velY, int frames, std::string imageFile)
{
	GameObject::Init(x, y, velX, velY, 16, 16);

	Effect::image = al_load_bitmap(imageFile.c_str());
	Effect::frames = frames;

	SetID(MISC, 0);

	maxFrame = al_get_bitmap_width(image) / tileSize;
	curFrame = 1;
	frameCount = 0;
	animationColumns = al_get_bitmap_width(image) / tileSize;
	facing = LEFT;
	animationDirection = 1;
	frameDelay = 10;

	frameHeight = tileSize;
	frameWidth = tileSize;
	z = 2;

}
void Effect::Destroy()
{
	GameObject::Destroy();
}
void Effect::Update()
{
	GameObject::Update();

	if (++frameCount > frameDelay)
	{
		curFrame += animationDirection;
		if (curFrame >= maxFrame)
			curFrame = 0;
		else if (curFrame <= 0)
			curFrame = maxFrame;

		if (forFrames > 0)
			forFrames--;

		frameCount = 0;
		frames--;

		if (frames <= 0)
			SetAlive(false);
		
	}
}
void Effect::Render()
{
	GameObject::Render();

	int fx = (curFrame % animationColumns) * frameWidth;
	int fy = 0;

	if (image)
		al_draw_bitmap_region(image, fx, fy, frameWidth, frameHeight, x - (frameWidth - tileSize), y - (frameHeight - tileSize), 0);
	else
		al_draw_rectangle(x, y, 2 * boundX, 2 * boundY, al_map_rgb(1, 100, 120), 2);
}
void Effect::Collided(GameObject *other)
{
	if (other->CheckID(PLAYER))
	{
		if (abs(x - other->GetX()) < tileSize / 3)
		{
			velX = 0;
			velY = 0;
			SetAlive(false);
		}
	}
}