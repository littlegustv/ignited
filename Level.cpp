#include "Level.h"

bool SortByY(GameObject *a, GameObject *b);

Level::Level(char *filename, void(*gotolevel)(int, int, int, bool), void(*DoAction)(std::string), void(*ChangeState)(int))
{
	ALLEGRO_DISPLAY *disp = al_get_current_display();
	ALLEGRO_BITMAP *backbuffer = al_get_backbuffer(disp);

	Level::DoAction = DoAction;
	Level::ChangeState = ChangeState;

	int playerX = 288;
	int playerY = 512;

	pugi::xml_document doc;

	pugi::xml_parse_result result = doc.load_file(filename);

	pugi::xml_node map = doc.child("map");

	int index = 0;

	//load tileset images

	for (pugi::xml_node tiles = map.child("tileset"); tiles; tiles = tiles.next_sibling("tileset"))
	{

		std::string tileF = tiles.child("image").attribute("source").value();
		tilesets[index] = al_load_bitmap(tileF.c_str());
		tilesetInfo[index][0] = atoi(tiles.child("image").attribute("width").value()) / tileSize;
		tilesetInfo[index][1] = atoi(tiles.attribute("firstgid").value());
		index++;
	}


	//convert tilemaps into backgrounds

	for (pugi::xml_node layer = map.child("layer"); layer; layer = layer.next_sibling("layer"))
	{
		layerWidth = atoi(layer.attribute("width").value());
		layerHeight = atoi(layer.attribute("height").value());
		int zi = 0;
		int scr = 0;

		pugi::xml_node zindex = layer.child("properties").find_child_by_attribute("name", "z-index");
		if (zindex)
			zi = atoi(zindex.attribute("value").value());
		else
			zi = 0;

		pugi::xml_node scroll = layer.child("properties").find_child_by_attribute("name", "scroll");
		if (scroll)
			scr = atoi(scroll.attribute("value").value());
		else
			scr = 0;

		//std::cout << zi << "\n";
		Background *b = new Background(layer.child("data").child_value(), backbuffer, tilesets, tilesetInfo, zi, layerWidth, layerHeight, 0, scr);
		objects.push_back(b);

	}

	//load objects and walls (non-passable)

	for (pugi::xml_node obj = map.child("objectgroup").child("object"); obj; obj = obj.next_sibling("object"))
	{
		int wx = (atoi(obj.attribute("x").value()) / tileSize);
		int wy = (atoi(obj.attribute("y").value()) / tileSize);
		int ww = (atoi(obj.attribute("width").value()) / tileSize);
		int wh = (atoi(obj.attribute("height").value()) / tileSize);

		for (int i = wx; i < wx + ww; i ++)
		{
			for (int j = wy; j < wy + wh; j ++)
			{
				if (strcmp(obj.attribute("type").value(), "Wall") == 0)
					solid[i][j] = true;

				else if (strcmp(obj.attribute("type").value(), "Safe") == 0)
					safe[i][j] = true;

				else if (strcmp(obj.attribute("type").value(), "Exit") == 0)
				{
					bool locked = !!atoi(obj.child("properties").find_child_by_attribute("name", "Status").attribute("value").value());
					int dest = atoi(obj.child("properties").find_child_by_attribute("name", "Destination").attribute("value").value());
					int tox = atoi(obj.child("properties").find_child_by_attribute("name", "PlayerX").attribute("value").value()) * tileSize;
					int toy = atoi(obj.child("properties").find_child_by_attribute("name", "PlayerY").attribute("value").value()) * tileSize;
					bool play = atoi(obj.child("properties").find_child_by_attribute("name", "Play").attribute("value").value()) > 0 ? true : false;
					std::string name = obj.attribute("name").value();

					Exit *e = new Exit(i*tileSize, j*tileSize, dest, gotolevel, tox,toy, locked, play);
					e->SetName(name);
					objects.push_back(e);
				}

				else if (strcmp(obj.attribute("type").value(), "NPC") == 0)
				{
					std::string imageFile = obj.child("properties").find_child_by_attribute("name", "ImageFile").attribute("value").value();
					std::string dataFile = obj.child("properties").find_child_by_attribute("name", "DataFile").attribute("value").value();
					std::string name = obj.attribute("name").value();
					std::string description = obj.child("properties").find_child_by_attribute("name", "Description").attribute("value").value();

					NPC *npc = new NPC(i*tileSize, j*tileSize, imageFile, dataFile, name, description);
					objects.push_back(npc);
				}

				else if (strcmp(obj.attribute("type").value(), "Item") == 0)
				{
					std::string imageFile = obj.child("properties").find_child_by_attribute("name", "ImageFile").attribute("value").value();
					std::string dataFile = obj.child("properties").find_child_by_attribute("name", "DataFile").attribute("value").value();
					int action = atoi(obj.child("properties").find_child_by_attribute("name", "Action").attribute("value").value());
					std::string name = obj.attribute("name").value();
					std::string actOn = obj.child("properties").find_child_by_attribute("name", "ActOn").attribute("value").value();
					std::string description = obj.child("properties").find_child_by_attribute("name", "Description").attribute("value").value();

					Item *it = new Item(i*tileSize, j*tileSize, imageFile, dataFile, name,description, action,actOn);
					objects.push_back(it);
				}

				else if (strcmp(obj.attribute("type").value(), "Hazard") == 0)
				{
					std::string imageFile = obj.child("properties").find_child_by_attribute("name", "ImageFile").attribute("value").value();
					std::string name = obj.attribute("name").value();

					Hazard *h = new Hazard(i*tileSize, j*tileSize, imageFile);
					h->SetName(name);
					objects.push_back(h);
				}

				else if (strcmp(obj.attribute("type").value(), "Obstacle") == 0)
				{
					std::string imageFile = obj.child("properties").find_child_by_attribute("name", "ImageFile").attribute("value").value();
					bool canMove = atoi(obj.child("properties").find_child_by_attribute("name", "CanMove").attribute("value").value());
					std::string name = obj.attribute("name").value();
					Obstacle *ob = new Obstacle(i*tileSize, j*tileSize, imageFile, canMove);
					ob->SetName(name);
					objects.push_back(ob);
				}

				else if (strcmp(obj.attribute("type").value(), "Cinematic") == 0)
				{
					std::string dataFile = obj.child("properties").find_child_by_attribute("name", "DataFile").attribute("value").value();
					Cinematic *cinem = new Cinematic(i*tileSize, j*tileSize, dataFile, DoAction, ChangeState);
					std::string name = obj.attribute("name").value();
					cinem->SetName(name);
					objects.push_back(cinem);
				}

				else if (strcmp(obj.attribute("type").value(), "Player") == 0)
				{
					playerX = i * tileSize;
					playerY = j * tileSize;
				}

			}
		}
	}

	al_init_font_addon();
	al_init_ttf_addon();

	player = new Player();

	playerImage = al_load_bitmap("./Pixelart/Char4.png");
	dialogueImage = al_load_bitmap("./Pixelart/dialoguebox.png");


	Level::font18 = al_load_font("./Data/SpecialElite.ttf", 18, 0);

	player->Init(playerImage, playerX, playerY, solid);
	objects.push_back(player);

	for (iter = objects.begin(); iter != objects.end(); ++iter)
	{
		(*iter)->SetSolid(solid);
	}
}

void Level::Destroy()
{
	for (iter = objects.begin(); iter != objects.end();)
	{
		(*iter)->Destroy();
		delete(*iter);
		iter = objects.erase(iter);
	}

//	al_destroy_bitmap(playerImage);
	al_destroy_bitmap(dialogueImage);

	for (int i = 0; i < 10; i++)
	{
		al_destroy_bitmap(tilesets[i]);
	}
}

void Level::Render()
{

	objects.sort(SortByY);

	for (iter = objects.begin(); iter != objects.end(); ++iter)
	{
		(*iter)->Render();
	}

}
void Level::Collide()
{
	for (iter = objects.begin(); iter != objects.end(); ++iter)
	{
		if (!(*iter)->Collideable()) continue;

		for (iter2 = iter; iter2 != objects.end(); ++iter2)
		{
			if (!(*iter2)->Collideable()) continue;
			/*						for (int i = 0; i < 10; i++)
			{
			if ((*iter)->CheckID((*iter2)->GetID(i)))
			continue;
			}*/

			if ((*iter)->CheckCollisions((*iter2)))
			{
				(*iter)->Collided((*iter2));
				(*iter2)->Collided((*iter));

				Player *pi = dynamic_cast<Player*>((*iter2));
				Cinematic *cine = dynamic_cast<Cinematic*>((*iter));

				if (pi && cine)
				{
					Level::ci = cine;
				}
				else
				{
					Player *pi = dynamic_cast<Player*>((*iter));
					Cinematic *cine = dynamic_cast<Cinematic*>((*iter2));
					
					if (pi && cine)
					{
						Level::ci = cine;
					}
				}
			}
		}
	}

}
void Level::Update()
{
	for (iter = objects.begin(); iter != objects.end(); ++iter)
	{
		(*iter)->Update();
		Obstacle *o = dynamic_cast<Obstacle*>((*iter));
		if (o && rand() % 1000 < 1)
		{
			o->Animate(1);
			DoAction("SHAKE");
		}
		SpreadFire((*iter));
	}
}
void Level::CleanUp()
{
	for (iter = objects.begin(); iter != objects.end();)
	{
		if (!(*iter)->GetAlive())
		{
			delete(*iter);
			iter = objects.erase(iter);
		}
		else
		{
			iter++;
		}
	}
}


bool SortByY(GameObject *a, GameObject *b)
{
	if (a->GetZ() == b->GetZ())
		return (a->GetY() < b->GetY());
	else
		return (a->GetZ() < b->GetZ());
}

bool Level::CheckInteract()
{
	for (iter = objects.begin(); iter != objects.end(); ++iter)
	{
		if ((*iter)->CheckID(INTERACT))
		{
			if ((player->GetFacing() == RIGHT && (*iter)->GetX() == player->GetX() + tileSize && player->GetY() == (*iter)->GetY()) ||
				(player->GetFacing() == LEFT && (*iter)->GetX() == player->GetX() - tileSize && player->GetY() == (*iter)->GetY()) ||
				(player->GetFacing() == UP && (*iter)->GetY() == player->GetY() - tileSize && player->GetX() == (*iter)->GetX()) ||
				(player->GetFacing() == DOWN && (*iter)->GetY() == player->GetY() + tileSize && player->GetX() == (*iter)->GetX()) ||
				(player->GetX() == (*iter)->GetX() && player->GetY() == (*iter)->GetY())
				)
			{
				interacting = (*iter);
				return true;
			}
		}
	}
	return false;
}

bool Level::SetInteract(std::string name)
{
	for (iter = objects.begin(); iter != objects.end(); ++iter)
	{
		if ((*iter)->GetName().compare(name) == 0)
		{
			interacting = (*iter);
			return true;
		}
	}
	return false;
}

void Level::SpreadFire(GameObject *o)
{
	Hazard *h = dynamic_cast<Hazard*> (o);
	if (h != NULL)
	{
		if (!h->CanSpawn())
			return;

		int nx = 0;
		int ny = 0;
		if (rand() % 100 == 0)
		{
			int chance = rand() % 2;
			if (chance == 0)
			{
				nx = h->GetX();
				chance = rand() % 2;
				if (chance == 0)
					ny = h->GetY() + tileSize;
				else
					ny = h->GetY() - tileSize;
			}
			else
			{
				ny = h->GetY();
				chance = rand() % 2;
				if (chance == 0)
					nx = h->GetX() + tileSize;
				else
					nx = h->GetX() - tileSize;
			}

			if (solid[nx / tileSize][ny / tileSize])
				return;

			if (safe[nx / tileSize][ny / tileSize])
				return;

			if (nx == player->GetX() && ny == player->GetY())
				return;

			if (abs(nx - player->GetX() < 4 && abs(ny - player->GetY() < 4)))
				return;

			for (iter2 = objects.begin(); iter2 != objects.end(); ++iter2)
			{
				Hazard *h2 = dynamic_cast<Hazard*> ((*iter2));
				if (h2 != NULL)
				{
					if (h2->GetX() == nx && h2->GetY() == ny)
						return;
				}

			}
			h->SetSpawned(true);
			Hazard *h1 = new Hazard(nx, ny, "./Pixelart/flames.png");
			objects.push_back(h1);
		}
	}
}

void Level::ChangeImage(std::string name, std::string imageFile)
{
	for (iter = objects.begin(); iter != objects.end(); iter++)
	{
		if ((*iter)->GetName().compare(name) == 0)
		{
			(*iter)->NewImage(imageFile);
			return;
		}
	}
}
void Level::ChangeData(std::string name, std::string dataFile)
{
	for (iter = objects.begin(); iter != objects.end(); iter++)
	{
		if ((*iter)->GetName().compare(name) == 0)
		{
			std::cout << "found!\n";
			(*iter)->NewData(dataFile);
			return;
		}
	}
	std::cout << "not found :(\n";
}

GameObject * Level::GetObject(std::string name)
{
	for (iter = objects.begin(); iter != objects.end(); iter++)
	{
		if ((*iter)->GetName().compare(name) == 0)
		{
			std::cout << "found!\n";
			return (*iter);
		}
	}
	std::cout << "not found.\n";
}