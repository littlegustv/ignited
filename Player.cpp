#include "Player.h"

Player::Player()
{
}
void Player::Init(ALLEGRO_BITMAP *image, int x, int y, bool solid[200][200])
{
	GameObject::Init(x, y, 0, 0, 16, 16);

	SetID(PLAYER, 0);

	Player::health = 3;

	for (int i = 0; i < 200; i++)
	{
		for (int j = 0; j < 200; j++)
		{
			Player::solid[i][j] = solid[i][j];
		}
	}
	Player::image = image;
	Player::heart = al_load_bitmap("./Pixelart/heart.png");

	maxFrame = 3;
	curFrame = 1;
	frameCount = 0;
	animationColumns = 3;
	facing = RIGHT;

	frameHeight = al_get_bitmap_height(Player::image) / 4;
	frameWidth = al_get_bitmap_width(Player::image) / 3;
	z = 1;

	animationDirection = 1;
	moveSpeed = baseSpeed;

	name = "Claire";
}

void Player::Destroy()
{
	velX = 0;
	velY = 0;
	al_destroy_bitmap(heart);
	GameObject::Destroy();
}
void Player::Render()
{
	GameObject::Render();
	int fx = (curFrame % animationColumns) * frameWidth;
	int fy = facing * frameHeight;

//	if (debug)
//		al_draw_rectangle(x, y, x + 2 * boundX, y + 2 * boundY, al_map_rgb(255, 200, 100), 2);
	if (hurtTime >= 0)
	{

		al_draw_tinted_bitmap_region(image, al_map_rgba_f(1.0,0.8,0.8,1.0), fx, fy, frameWidth, frameHeight, x, y - (frameHeight - tileSize), 0);
		for (int i = 0; i < health; i++)
		{
			//al_draw_filled_rectangle(x + i * 8, y + 2, x + i * 8 + 6, y + 8, al_map_rgb(255, 0, 0));
			al_draw_bitmap(heart, x + i * 10, y - 4, 0);
		}
	}
	else
		al_draw_bitmap_region(image, fx, fy, frameWidth, frameHeight, x, y - (frameHeight - tileSize), 0);

	//al_draw_filled_rounded_rectangle(x, y, x + 10, y + 10, 5, 5, al_map_rgb(200, 100, 220));
}
void Player::Update()
{
	frameDelay = 20 / moveSpeed;
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

	if (hurtTime >= 0)
		hurtTime--;
}

void Player::Move(int direction)
{
	switch (direction)
	{
	case UP:
		if (velX == 0)
		{
			facing = UP;
			if (!solid[x / tileSize][y / tileSize])
			{
				velY = -moveSpeed;
			}
			else
				velY = 0;
		}
		break;
	case DOWN:
		if (velX == 0)
		{
			facing = DOWN;
			if (!solid[x / tileSize][y / tileSize + 1])
			{
				velY = moveSpeed;
			}
			else
				velY = 0;
		}
		break;
	case RIGHT:
		if (velY == 0)
		{
			facing = RIGHT;
			if (!solid[x / tileSize + 1][y / tileSize])
			{
				velX = moveSpeed;
			}
			else
				velX = 0;
		}
		break;
	case LEFT:
		if (velY == 0)
		{
			facing = LEFT;
			if (!solid[x / tileSize][y / tileSize])
			{
				velX = -moveSpeed;
			}
			else
				velX = 0;
		}
		break;
	}
}

void Player::Collided(GameObject *other)
{
//	int otherID = other->GetID();
	if (other->CheckID(SOLID))
	{
		velY = -velY;
		velX = -velX;
		/*
		if (other->GetY() > y)
		{
			velY = velY > 0 ? 0 : velY;
		}
		else if (other->GetY() <= y)
		{
			velY = velY < 0 ? 0 : velY;
		}

		if (other->GetX() > x)
		{
			velX = velX > 0 ? 0 : velX;
		}
		else if (other->GetX() <= x)
		{
			velX = velX < 0 ? 0 : velX;
		}*/
		ResetAnimation(1);
		ResetAnimation(0);
	}
	if (other->CheckID(HAZARD))
	{
		if (hurtTime <= 0)
		{
			velY = - 2 * velY;
			velX = - 2 * velX;
			health--;
			hurtTime = 50;
		}
	}
	if (other->CheckID(EXIT))
	{
		//Check if has key, if so:unlock and drop
		Exit *e = dynamic_cast<Exit*> (other);
		if (e != NULL)
		{
			if (e->IsLocked())
			{
				for (iter = inventory.begin(); iter != inventory.end(); ++iter)
				{
					if ((*iter)->GetActOn().compare(e->GetName()) == 0)
					{
						e->SetLocked(false);
						inventory.remove((*iter));
						return;
					}
				}
			}
		}
		else
		{
			std::cout << "Casting didn't work...\n";
		}

	}
}

void Player::ResetAnimation(int flag)
{
	if (forFrames > 0)
	{
		return;
	}
	if (flag == 1)
	{
		if (y % tileSize <= baseSpeed)
		{
			velY = 0;
			y = (y / tileSize) * tileSize;
		}
		else if (y % tileSize >= tileSize - baseSpeed)
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
		else if (x % tileSize >= tileSize - baseSpeed)
		{
			velX = 0;
			x = (x / tileSize + 1) * tileSize;
		}
	/*	else if (x % tileSize < tileSize / 2)
		{
			velX = -baseSpeed;
		}
		else if (x % tileSize > tileSize / 2)
		{
			velX = baseSpeed;
		}*/
	}

	if (velY == 0 && velX == 0)
	{
		animationDirection = 0;
		curFrame = 1;
	}
	else
		animationDirection = 1;
}

DialogueTree * Player::InventoryTree()
{
	DialogueTree *t = new DialogueTree();
	t->text = "Inventory";
	int index = 0;

	//std::cout << inventory.front();

	for (iter = inventory.begin(); iter != inventory.end(); ++iter)
	{
	//	std::cout << "ZOMG" << (*iter)->GetName();
		t->choices[index] = (*iter)->GetName();
		DialogueTree *temp = new DialogueTree();
		temp->text = (*iter)->GetName();
	//	temp->choices[0] = "Use";
		temp->choices[1] = "Drop";
		temp->choices[0] = "Look";
	//	temp->branches[0] = new DialogueTree();
		temp->branches[1] = new DialogueTree();
		temp->branches[0] = new DialogueTree();
	//	temp->branches[0]->text = "ACTION";
	//	temp->branches[0]->choices[0] = "USE";
		temp->branches[1]->text = "ACTION";
		temp->branches[1]->choices[0] = "DROP";
		temp->branches[0] = (*iter)->GetTree();
		temp->branches[0]->active = true;
		temp->active = true;
		t->branches[index] = temp;
	//	std::cout << t->choices[index];
		index++;
	}

	t->choices[index] = "Exit Inventory";
	DialogueTree *temp = new DialogueTree();
	temp->text = "DONE";
	t->branches[index] = temp;
	

	t->active = true;

	//std::cout << t->text << " : " << t->choices[0] << "\n";

	return t;
}

Item * Player::DropItem(std::string nm)
{
	for (iter = inventory.begin(); iter != inventory.end(); ++iter)
	{
		if ((*iter)->GetName().compare(nm) == 0)
		{
			Item *o = (*iter);
			inventory.remove((*iter));
			return o;
		}
	}
	return NULL;
	 
}

Item * Player::GetItem(std::string nm)
{
	for (iter = inventory.begin(); iter != inventory.end(); ++iter)
	{
		if ((*iter)->GetName().compare(nm))
		{
			Item *o = (*iter);
			return o;
		}
	}
	return NULL; 
}