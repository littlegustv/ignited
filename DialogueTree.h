#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

class DialogueTree
{
private:
public:
	bool active = false;

	int c = 0;

	std::string text = "";
	std::string choices[20];
	DialogueTree *branches[20];

	DialogueTree()
	{
		text = "";
		for (int i = 0; i < 20; i++)
		{
			choices[i] = "";
			branches[i] = NULL;
		}
		choices[0] = "...";
	}
	
	void LoadData(std::ifstream &b_file, DialogueTree *current);
	
};