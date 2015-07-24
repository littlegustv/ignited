#pragma once

#include "GameObject.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

class Cinematic : public GameObject
{
private:
	std::string actions[1000];
	std::string dataFile;
	void(*DoAction)(std::string);
	void(*ChangeState)(int newState);

public:
	Cinematic(int x, int y, std::string dataFile, void(*DoAction)(std::string), void(*ChangeState)(int));
	/*
	void SetDoAction(void(*DoAction)(std::string)){ Cinematic::DoAction = DoAction; }
	void SetChangeState(void(*ChangeState)(int)){ Cinematic::ChangeState = ChangeState; }
	*/
	void Destroy();
	void Update(int cid);
	void Update() {}
	void Render();
	void Collided(GameObject *other);
};