#pragma once

#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>

#include "GameObject.h"
#include "DialogueTree.h"

class Dialogue : public GameObject
{
private:
	bool show;
	DialogueTree *tree;
	int choice;
	int maxChoice;
	ALLEGRO_FONT *myfont;

	int px;
	int py;
	ALLEGRO_TRANSFORM trans;
	std::string chosen;
	std::string actOn;
	std::string actData;

public:
	Dialogue(DialogueTree *tree, ALLEGRO_BITMAP *image, ALLEGRO_FONT *myfont, int x, int y, std::string name = "");
	void Destroy();
	void Update();
	void Render();
	void Collided();

	void MoveChoice(int flag);
	int MakeChoice();

	DialogueTree *GetTree() { return tree; }

	void SetMaxChoice();
	std::string GetChosen() { return chosen; }
	void SetChosen(std::string c) { chosen = c; }

	std::string GetActOn() { return actOn; }
	std::string GetActData() { return actData; }

};