#include "NPC.h"

NPC::NPC(int x, int y, std::string imageFile, std::string dataFile, std::string name, std::string description)
{
	GameObject::Init(x, y, 0, 0, 16, 16);

	SetID(INTERACT, 0);
	SetID(SOLID, 1);

	NPC::image = al_load_bitmap(imageFile.c_str());
	NPC::name = name;
	NPC::description = description;

	maxFrame = 3;
	curFrame = 1;
	frameCount = 0;
	animationColumns = 3;
	facing = LEFT;
	frameDelay = 6;

	frameHeight = al_get_bitmap_height(NPC::image) / 4;
	frameWidth = al_get_bitmap_width(NPC::image) / 3;
	z = 1;

	NPC::imageFile = imageFile;
	NPC::dataFile = dataFile;

	std::ifstream b_file(dataFile);

	DialogueTree *d = new DialogueTree();

	d->LoadData(b_file, d);

	NPC::tree = d;

	moveSpeed = baseSpeed;

}

void NPC::Destroy()
{
	GameObject::Destroy();
}

void NPC::Render()
{
	GameObject::Render();

	int fx = (curFrame % animationColumns) * frameWidth;
	int fy = facing * frameHeight;

	al_draw_bitmap_region(image, fx, fy, frameWidth, frameHeight, x - (frameWidth - tileSize), y - (frameHeight - tileSize), 0);


	//al_draw_filled_rectangle(x-boundX, y-boundY, x+boundX, y+boundY, al_map_rgb(50,200,100));

	//al_draw_bitmap(image, x, y, 0);

}
void NPC::Update()
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
	}
}

void NPC::Move(int direction)
{
	switch (direction)
	{
	case UP:
		if (velX == 0)
		{
			facing = UP;
			velY = -moveSpeed;
		}
		break;
	case DOWN:
		if (velX == 0)
		{
			facing = DOWN;
			velY = moveSpeed;
		}
		break;
	case RIGHT:
		if (velY == 0)
		{
			facing = RIGHT;
			velX = moveSpeed;
		}
		break;
	case LEFT:
		if (velY == 0)
		{
			facing = LEFT;
			velX = -moveSpeed;
		}
		break;
	}
}

void NPC::Collided(GameObject *other)
{

}

void NPC::ResetAnimation(int flag)
{
	if (flag == 1)
	{
		if (y % tileSize <= baseSpeed)
		{
			velY = 0;
			y = (y / tileSize) * tileSize;
		}
		else
		{
			velY = 0;
			y = (y / tileSize + 1) * tileSize;
		}

	}
	else if (flag == 0)
	{
		if (x % tileSize <= baseSpeed)
		{
			velX = 0;
			x = (x / tileSize) * tileSize;
		}
		else
		{
			velX = 0;
			x = (x / tileSize + 1) * tileSize;
		}
	}

	if (velY == 0 && velX == 0)
	{
		animationDirection = 0;
		curFrame = 1;
	}
	else
		animationDirection = 1;

}