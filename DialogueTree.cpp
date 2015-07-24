#include "DialogueTree.h"

void DialogueTree::LoadData(std::ifstream &b_file, DialogueTree *current)
{
	if (b_file.is_open())
	{

		if (!b_file.eof())
		{
			std::string line;
			getline(b_file, line);

			line.erase(std::remove(line.begin(), line.end(), '\t'), line.end());

			if (line[0] == '-')
			{
				if (!line.substr(1).empty())
				{
					current->text = line.substr(1);
					if (!current->active)
						current->active = true;
				}
				else
				{
					current->text = "That's it.";
				}
			}
			else if (line[0] == '{')
			{
				DialogueTree *temp = new DialogueTree();
				current->branches[current->c] = temp;
				current->choices[current->c] = !line.substr(1).empty() ? line.substr(1) : "...";
				current->c = (current->c + 1);
			//	std::cout << current->c;
				LoadData(b_file, temp);
			}
			else if (line[0] == '}')
				return;
			LoadData(b_file, current);

		}
		else
			return;
	}
	else
		return;

}