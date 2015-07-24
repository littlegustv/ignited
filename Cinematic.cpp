#include "Cinematic.h"

Cinematic::Cinematic(int x, int y, std::string dataFile, void(*DoAction)(std::string), void(*ChangeState)(int))
{
	GameObject::Init(x, y, 0, 0, 16, 16);

	Cinematic::DoAction = DoAction;
	Cinematic::ChangeState = ChangeState;

	std::ifstream b_file(dataFile);

	if (b_file.is_open())
	{
		while (!b_file.eof())
		{
			std::string line;
			getline(b_file, line);
			//std::cout << line << "\n";
			int k = line.find_first_of(":", 0);
			int index = atoi(line.substr(0, k).c_str());
			//make sure it is in bounds...
			if (index >= 1000)
				continue;
			std::string action = line.substr(k + 1);
			actions[index] = action;

		}
	}

	GameObject::SetAlive(true);

}
void Cinematic::Destroy()
{
	GameObject::Destroy();
	//SetAlive(false);
}
void Cinematic::Update(int cid)
{
	GameObject::Update();

	if (!actions[cid].empty())
	{
		DoAction(actions[cid]);
	}

}
void Cinematic::Render()
{
	GameObject::Render();
	//al_draw_rectangle(x, y, x + 32, y + 32, al_map_rgb(100, 200, 200), 2);
}
void Cinematic::Collided(GameObject *other)
{
	if (other->CheckID(PLAYER))
	{
		SetCollideable(false);
		ChangeState(CINEMATIC);
	}
}