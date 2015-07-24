#include "Obstacle.h"

Obstacle::Obstacle(int x, int y, std::string imageFile, bool canMove)
{
	SetID(SOLID, 0);

	Obstacle::canMove = canMove;

	maxFrame = 6;
	curFrame = 0;
	frameCount = 0;
	frameDelay = 8;
	animationColumns = 6;
	animationDirection = 0;
	z = 1;

	Obstacle::imageFile = imageFile;
	Obstacle::image = al_load_bitmap(imageFile.c_str());

	frameHeight = al_get_bitmap_height(Obstacle::image);
	frameWidth = al_get_bitmap_width(Obstacle::image) / 6;
	GameObject::Init(x, y, 0, 0, frameWidth / 2, 16);

}
void Obstacle::Destroy()
{
	GameObject::Destroy();
}
void Obstacle::Update()
{
	GameObject::Update();
	
	if (++frameCount > frameDelay)
	{
		curFrame += animationDirection;
		if (curFrame >= maxFrame)
		{
			animationDirection = 0;
			curFrame = maxFrame - 1;
		}
		frameCount = 0;
	}
	
	if (animationDirection == 0 && pushed != -1)
	{
		switch (pushed)
		{
		case UP:
			if (y % tileSize < tileSize / 4)
			{
				y = tileSize * (y / tileSize);
				velY = 0;
				velX = 0;
			}
			break;
		case DOWN:
			if (y % tileSize > 3 * tileSize / 4)
			{
				y = tileSize * (y / tileSize + 1);
				velY = 0;
				velX = 0;
			}
			break;
		case RIGHT:
			if (x % tileSize > 3 * tileSize / 4)
			{
				x = tileSize * (x/ tileSize + 1);
				velY = 0;
				velX = 0;
			}
			break;
		case LEFT:
			if (x % tileSize < tileSize / 4)
			{
				x = tileSize * (x / tileSize);
				velY = 0;
				velX = 0;
			}
			break;
		}
	}
	

}
void Obstacle::Render()
{
	GameObject::Render();

	int fx = (curFrame % animationColumns) * frameWidth;
	int fy = 0;// facing * frameHeight;

	al_draw_bitmap_region(image, fx, fy, frameWidth, frameHeight, x -(frameWidth - tileSize), y- tileSize, 0);

	//al_draw_rectangle(x, y, x + 2 * boundX, y + 2 * boundY, al_map_rgb(200, 100, 100), 2);
}
void Obstacle::Collided(GameObject *other)
{
	if (other->CheckID(PLAYER))
	{
		if (animationDirection == 0 && curFrame < 1)
			animationDirection = 1;
		else if (animationDirection == 0 && canMove)
		{
			//PUSH!
			if (other->GetY() > y && !solid[x/tileSize][y/tileSize - 1])
			{
				pushed = UP;
				velY = -baseSpeed / 2;
			}
			else if (other->GetY() < y && !solid[x / tileSize][y/ tileSize + 1])
			{
				pushed = DOWN;
				velY = baseSpeed / 2;
			}
			else if (other->GetX() > x && !solid[x/ tileSize - 1][y / tileSize])
			{
				pushed = LEFT;
				velX = -baseSpeed / 2;
			}
			else if (other->GetX() < x && !solid[x / tileSize + 1][y / tileSize])
			{
				pushed = RIGHT;
				velX = baseSpeed / 2;
			}

		}
	}
}