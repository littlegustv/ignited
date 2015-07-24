#include "Item.h"

Item::Item(int x, int y, std::string imageFile, std::string dataFile, std::string name, std::string description, int action, std::string actOn)
{
	GameObject::Init(x, y, 0, 0, 32, 32);
	
	SetID(INTERACT, 0);
	
	Item::image = al_load_bitmap(imageFile.c_str());
	Item::name = name;

	Item::description = !description.empty() ? description : "There is nothing of interest to see.";

	frameHeight = 32;
	frameWidth = 32;

	Item::dataFile = dataFile;
	Item::imageFile = imageFile;

	DialogueTree *d = new DialogueTree();
	
	d->text = description;
	d->branches[0] = new DialogueTree();
	d->branches[0]->text = "ACTION";
	d->branches[0]->choices[0] = "GET";
	d->choices[0] = "Pick Up";
	d->choices[1] = "Cancel";
	d->active = true;

	Item::tree = d;
	
	Item::action = action;
	Item::actOn = actOn;
}
void Item::Destroy()
{
	GameObject::Destroy();
}
void Item::Update()
{
	GameObject::Update();
}
void Item::Render()
{
	//al_draw_circle(x, y, boundX / 2, al_map_rgb(200, 100, 200), 2);
	al_draw_bitmap(image, x, y, 0);
}
void Item::Collided()
{}

bool Item::DoActions(GameObject *interact)
{
	if (interact->GetName().compare(actOn) != 0)
	{
		return false;
	}

	return true;
}

void Item::SetTree()
{
	DialogueTree *d = new DialogueTree();

	std::ifstream b_file(dataFile);

	d->LoadData(b_file, d);

	Item::tree = d;
}

void Item::ResetTree()
{
	DialogueTree *d = new DialogueTree();

	d->text = Item::description;
	d->branches[0] = new DialogueTree();
	d->branches[0]->text = "ACTION";
	d->branches[0]->choices[0] = "GET";
	d->choices[0] = "Pick Up";
	d->choices[1] = "Cancel";
	d->active = true;

	Item::tree = d;
}