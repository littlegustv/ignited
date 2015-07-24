#include "GameObject.h"

GameObject::GameObject()
{
	for (int i = 0; i < 10; i++)
	{
		id[i] = -1;
	}
	x = 0;
	y = 0;

	velX = 0;
	velY = 0;

	boundX = 0;
	boundY = 0;

	maxFrame = 0;
	curFrame = 0;
	frameCount = 0;
	frameDelay = 0;
	frameWidth = 0;
	frameHeight = 0;
	animationColumns = 0;
	animationDirection = 0;

	image = NULL;

	alive = true;
	collideable = true;
}

void GameObject::Destroy()
{
	if (image)
		al_destroy_bitmap(image);
}

void GameObject::Init(int x, int y, int velX, int velY, int boundX, int boundY)
{
	GameObject::x = x;
	GameObject::y = y;

	GameObject::velX = velX;
	GameObject::velY = velY;

	GameObject::boundX = boundX;
	GameObject::boundY = boundY;
}

void GameObject::Update()
{
	x += velX;
	y += velY;
}

void GameObject::Render()
{
}

bool GameObject::CheckCollisions(GameObject *otherObject)
{
	float oX = otherObject->GetX();
	float oY = otherObject->GetY();

	int oBX = otherObject->GetBoundX();
	int oBY = otherObject->GetBoundY();
	/*
	if (x + boundX > oX - oBX &&
		x - boundX < oX + oBX &&
		y + boundY > oY - oBY &&
		y - boundY < oY + oBY)
	{
		return true;
	}
	else
		return false;
		*/
	if (x + 2 * boundX > oX &&
		x < oX + 2 * oBX &&
		y + 2 * boundY > oY &&
		y < oY + 2* oBY)
	{
		return true;
	}
	else
		return false;

}

void GameObject::Collided(GameObject *other)
{}

bool GameObject::Collideable()
{
	return alive && collideable;
}

bool GameObject::CheckID(int check)
{
	for (int i = 0; i < 10; i++)
	{
		if (GameObject::id[i] == check)
			return true;
	}

	return false;
}

bool GameObject::DoActions(GameObject *interact)
{
	return false;
}

void GameObject::NewData(std::string dataFile)
{ 
	GameObject::dataFile = dataFile;
	std::ifstream b_file(GameObject::dataFile);
	DialogueTree *d = new DialogueTree();
	d->LoadData(b_file, d);
	GameObject::tree = d;
}

void GameObject::NewImage(std::string imageFile) 
{ 
	GameObject::imageFile = imageFile;
	std::cout << "iamge ; " << imageFile << "\n";
	ALLEGRO_BITMAP *temp = al_load_bitmap(GameObject::imageFile.c_str());
	if (temp)
	{
		std::cout << "success!";
		GameObject::image = temp;
	}
}

void GameObject::SetSolid(bool solid[200][200]) {
	for (int i = 0; i < 200; i++)
	{
		for (int j = 0; j < 200; j++)
		{
			GameObject::solid[i][j] = solid[i][j];
		}
	}
}