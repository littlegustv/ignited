#pragma once

#include "GameObject.h"
#include <list>
#include "DialogueTree.h"
#include "Exit.h"
#include "Item.h"

class Player : public GameObject
{
private:
	std::list<Item *> inventory;
	std::list<Item *>::iterator iter;
	int health = 3;
	int moveSpeed;
	int hurtTime = 0;
	ALLEGRO_BITMAP *heart;
//	void(*DoAction)();

public:
	Player();
	void Init(ALLEGRO_BITMAP *image, int x, int y, bool solid[200][200]);

	void Destroy();

	void Render();
	void Update();

	void Move(int direction);

	void Collided(GameObject *other);

	void ResetAnimation(int flag);

	void AddItem(Item *object) { inventory.push_back(object); }
	Item *DropItem(std::string nm);
	Item *GetItem(std::string nm);
	DialogueTree * InventoryTree();

	int GetHealth() { return health; }
	
	void SetInventory(std::list<Item *> inventory) { Player::inventory = inventory; }
	std::list<Item*> GetInventory() { return inventory; }

	void SetSpeed(int speed) { moveSpeed = speed; }
	bool IsMoving() { return velX != 0 || velY != 0; }

	bool CheckSolid(int x, int y) { return solid[x / tileSize][y / tileSize]; }

};