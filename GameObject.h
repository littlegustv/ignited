#pragma once

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "globals.h"
#include "DialogueTree.h"

class GameObject
{
private:
	int id[10];
	bool alive;
	bool collideable;


protected:
	int x;
	int y;
	int z = 0;

	int velX;
	int velY;

	int boundX;
	int boundY;

	int maxFrame;
	int curFrame;
	int frameCount;
	int frameDelay;
	int frameWidth;
	int frameHeight;
	int animationColumns;
	int facing;
	int animationDirection;

	ALLEGRO_BITMAP *image;

	std::string name = "";
	std::string description;

	std::string imageFile;
	std::string dataFile;

	DialogueTree *tree;

	int forFrames;

	bool solid[200][200];

public:

	GameObject();
	void virtual Destroy();

	void Init(int x, int y, int velX, int velY, int boundX, int boundY);
	void virtual Update();
	void virtual Render();

	int GetX() { return x; }
	int GetY() { return y; }
	int GetZ() { return z; }

	void SetX(int x) { GameObject::x = x; }
	void SetY(int y) { GameObject::y = y; }

	int GetBoundX() { return boundX; }
	int GetBoundY() { return boundY; }

	int GetID(int index) { return id[index]; }
	void SetID(int id, int index) { GameObject::id[index] = id; }
	bool CheckID(int check);

	bool GetAlive() { return alive; }
	void SetAlive(bool alive) { GameObject::alive = alive; }

	bool GetCollideable() { return collideable; }
	void SetCollideable(bool collideable) { GameObject::collideable = collideable; }

	int GetFacing() { return facing; };
	void SetFacing(int face) { GameObject::facing = face; }

	std::string GetName() { return name; }
	void SetName(std::string name) { GameObject::name = name; }

	DialogueTree* GetTree() { return GameObject::tree; }
	void SetTree(DialogueTree *tree) { GameObject::tree = tree; }

	bool CheckCollisions(GameObject *otherObject);
	void virtual Collided(GameObject *other);
	bool Collideable();

	bool virtual DoActions(GameObject *interact);

	std::string GetDescription() { return description; }
	void SetDescription(std::string description) { GameObject::description = description; }

	void NewImage(std::string imageFile);
	void NewData(std::string dataFile); 

	void Animate(int frames) { GameObject::forFrames = frames; GameObject::animationDirection = 1; }

	void SetSolid(bool solid[200][200]);
};