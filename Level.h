#pragma once

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <list>
#include <math.h>

#include "globals.h"
#include "GameObject.h"
#include "Player.h"
#include "DialogueTree.h"
#include "Dialogue.h"
#include "NPC.h"
#include "Item.h"
#include "Background.h"
#include "Hazard.h"
#include "pugixml.hpp"
#include "Exit.h"
#include "Obstacle.h"
#include "Cinematic.h"
#include "Effect.h"

class Level
{
private:

	std::list<GameObject *> objects;
	std::list<GameObject *>::iterator iter;
	std::list<GameObject *>::iterator iter2;

	/*Game images - may turn to list or array*/
	ALLEGRO_BITMAP *playerImage = NULL;
	ALLEGRO_BITMAP *wallImage = NULL;
	ALLEGRO_BITMAP *dialogueImage = NULL;
	ALLEGRO_BITMAP *npcImage = NULL;
	ALLEGRO_BITMAP *itemImage = NULL;
	ALLEGRO_BITMAP *fireImage = NULL;

	/*Fonts!  just one for now...*/
	ALLEGRO_FONT *font18 = NULL;

	void(*DoAction)(std::string);
	void(*ChangeState)(int newState);

public:
	Player *player;
	Dialogue *di = NULL;
	Cinematic *ci = NULL;
	GameObject *interacting = NULL;
	ALLEGRO_BITMAP *tilesets[10];
	int tilesetInfo[10][2];
	Background *layers[10];
	bool solid[200][200];
	bool safe[200][200];

	int layerWidth;
	int layerHeight;

	void(*GoToLevel)(int l);

	Level(char *filename, void(*gotolevel)(int, int, int, bool), void(*DoAction)(std::string), void(*ChangeState)(int));
	void Destroy();
	void Render();
	void Collide();
	void Update();
	void CleanUp();
	void NewDialogue(int x, int y) { di = new Dialogue(interacting->GetTree(), dialogueImage, font18, x, y, interacting->GetName()); }
	void NewDialogue(int x, int y, DialogueTree *t) { di = new Dialogue(t, dialogueImage, font18, x, y); }
	bool CheckInteract();
	bool SetInteract(std::string name);
	void RemoveObject(GameObject *o) { objects.remove(o); }
	void AddObject(GameObject *o) { objects.push_back(o); }
	GameObject * GetObject(std::string name);
	void SpreadFire(GameObject *o);

	void ChangeImage(std::string name, std::string imageFile);
	void ChangeData(std::string name, std::string dataFile);

};