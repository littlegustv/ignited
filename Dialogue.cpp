#include "Dialogue.h"

Dialogue::Dialogue(DialogueTree *tree, ALLEGRO_BITMAP *image, ALLEGRO_FONT *myfont, int x, int y, std::string name)
{
	GameObject::Init(x + 10, y + HEIGHT - 138, 0, 0, WIDTH, 100);

	SetID(MISC, 0);

	Dialogue::image = image;
	Dialogue::tree = tree;
	Dialogue::myfont = myfont;
	Dialogue::name = name;

	SetMaxChoice();
}
void Dialogue::Destroy()
{
	GameObject::Destroy();
}
void Dialogue::Update()
{
	GameObject::Update();
}
void Dialogue::Render()
{
//	al_draw_rectangle(x, y, x + boundX, y + boundY, al_map_rgb(200, 200, 140), 2);

	al_draw_bitmap(image, x, y, 0);

	if (!name.empty())
		al_draw_textf(myfont, al_map_rgb(204, 169, 135), x + 20, y + 12, 0, "%s - %s", name.c_str(), tree->text.c_str());
	else
	{
		al_draw_textf(myfont, al_map_rgb(204, 169, 135), x + 20, y + 12, 0, "%s", tree->text.c_str());
	}
	if (tree && tree->active)
	{
		if (!tree->text.empty())
		{
			//al_draw_textf(myfont, al_map_rgb(204, 169, 150), x + 20, y + 34, 0, "\"%s\"", tree->text.c_str());

			al_draw_line(x + 8, y + 32, x + al_get_bitmap_width(image) - 20, y + 32, al_map_rgb(204, 169, 150), 1);
			al_draw_line(x + 8, y + 33, x + al_get_bitmap_width(image) - 20, y + 33, al_map_rgb(100, 55, 55), 1);

			for (int i = 0; i < 3; i++)
			{
				int j = choice > 2 ? choice - 2 : 0;
				if (i + j == choice)
				{
					al_draw_text(myfont, al_map_rgb(204, 100, 100), x + 10, y + 36 + 20 * i, 0, ">");
					al_draw_textf(myfont, al_map_rgb(240, 100, 100), x + 20, y + 36 + 20 * i, 0, "%s", tree->choices[i + j].c_str());
				}
				else if (!tree->choices[i + j].empty())
					al_draw_textf(myfont, al_map_rgb(100, 100, 200), x + 20, y + 36 + 20 * i, 0, "%s", tree->choices[i + j].c_str());
			}
		}
	}

}
void Dialogue::Collided()
{}

void Dialogue::MoveChoice(int flag)
{
	chosen = tree->text;
	if (flag == 1)
	{
		choice = (choice + 1) % maxChoice;
	}
	else
	{
		choice = choice > 0 ? choice - 1 : (maxChoice - 1);
	}
//	std::cout << "C: " << tree->text << " \n";
}
int Dialogue::MakeChoice()
{
//	if (!tree->branches[choice])
	//	return NONE;

	if (!tree)
		return NONE;

	if (!tree->branches[choice])
		return NONE;

	std::string text = tree->branches[choice]->text;

	if (text.compare("ACTION") == 0)
	{
		Dialogue::tree = tree->branches[choice];
		return ACTION;
	}
	else if (tree->branches[choice] && tree->branches[choice]->active)
	{
		Dialogue::tree = tree->branches[choice];
		//	std::cout << tree->text << "\n";
		Dialogue::choice = 0;
		SetMaxChoice();
		return NEXT;
	}
	else
		return NONE;
}

void Dialogue::SetMaxChoice()
{
	if (tree && tree->active)
	{
		maxChoice = 1;
		for (int i = 0; i < 4; i++)
		{
			if (!tree->choices[i].empty())
				maxChoice = i + 1;
		}
	}
}