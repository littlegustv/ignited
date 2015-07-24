#pragma once

#include "GameObject.h"
#include "DialogueTree.h"

class NPC : public GameObject
{
private:
	void NPC::LoadData(std::ifstream &b_file, DialogueTree *current);
	bool solid[200][200];
	int moveSpeed;

public:
	//DialogueTree *tree;

	NPC(int x, int y, std::string imageFile, std::string dataFile, std::string name, std::string description);

	void Destroy();

	void Render();
	void Update();

	void Move(int direction);

	void Collided(GameObject *other);

	void ResetAnimation(int flag);
};