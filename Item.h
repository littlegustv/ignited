#pragma once

#include "GameObject.h"

class Item : public GameObject
{
private:
	int action;
	std::string actOn;

public:
	Item(int x, int y, std::string imageFile, std::string dataFile, std::string name,std::string description, int action, std::string actOn);
	//Item(int x, int y, char *name, char *imageFile, char *dataFile);
	void Destroy();
	void Update();
	void Render();
	void Collided();
	void LoadData(std::ifstream &b_file, DialogueTree *current);
	bool DoActions(GameObject *interact);
	std::string GetActOn() { return actOn; }
	void SetActON(std::string actOn){ Item::actOn = actOn; }
	void SetTree();
	void ResetTree();

};