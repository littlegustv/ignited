#include "Exit.h"

Exit::Exit(int x, int y, int toLevel, void(*gotolevel)(int,int,int,bool), int playerX, int playerY, bool locked, bool play)
{
	GameObject::Init(x, y, 0, 0, 16, 16);
	SetID(EXIT, 0);
	SetID(SOLID, 1);
	SetID(INTERACT, 2);

	Exit::toLevel = toLevel;
	Exit::GoToLevel = gotolevel;
	Exit::toX = playerX;
	Exit::toY = playerY;
	Exit::name = "Exit";
	Exit::locked = locked;
	Exit::tree = new DialogueTree();
	Exit::description = locked ? "locked." : "unlocked.";
	Exit::tree->text = Exit::description;
	Exit::tree->active = true;
	Exit::play = play;
}
void Exit::Destroy()
{
	GameObject::Destroy();
}
void Exit::Render()
{

	//al_draw_rectangle(x, y, x + 2 * boundX, y + 2 * boundY, al_map_rgb(0, 0, 255),2);
}
void Exit::Update()
{
	GameObject::Update();
	Exit::description = locked ? "(Locked)" : "(Unlocked)";
	Exit::tree->text = Exit::description;
}
void Exit::Collided(GameObject *other)
{
	//std::cout << other->GetID(0);
	if (other->CheckID(PLAYER))
	{
		//std::cout << "ok!";
		if (!locked)
			GoToLevel(toLevel, toX, toY, play);
		else
		{
			showMessage = 2.0;
		}
	}
}