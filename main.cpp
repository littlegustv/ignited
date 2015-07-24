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
#include "Level.h"
#include "Camera.h"
#include "Cinematic.h"

bool keys[] = { false, false, false, false, false, false };
//current level
Level *l1 = NULL;

char *ls[] = { "./Data/bedroom.tmx", "./Data/upstairshallway.tmx", "./Data/level1.tmx", "./Data/outside1.tmx", "./Data/kitchen.tmx", "./Data/trees.tmx", "./Data/courtyard1.tmx" };
Level *levels[100];
Camera *c = NULL;

//Cinematic *ci = NULL;
int cin = 0;

bool debug = false;

int gameState = TITLE;

void GoToLevel(int newLevel, int toX, int toY, bool play = true);
void ChangeState(int newState);
void InitNew();
void ScreenShake();
void DoAction(std::string text);
//globals
//Player *player;
/*
std::list<GameObject *> objects;
std::list<GameObject *>::iterator iter;
std::list<GameObject *>::iterator iter2;
*/
//prototypes
//void __cdecl GoToLevel();


ALLEGRO_SAMPLE *door = NULL;
ALLEGRO_BITMAP *fadeImage = NULL;
float fade = 0;
float shake = 0;
/*
ALLEGRO_SAMPLE_ID stepID;
bool stepping = false;
*/

void __cdecl TakeLife();
void __cdecl ScorePoint();

//void ChangeState(int &state, int newState);
//bool SortByY(GameObject *a, GameObject *b);
//ALLEGRO_SAMPLE_INSTANCE *songInstance;
//ALLEGRO_BITMAP *playerImage = NULL;


int main(void)
{
	bool done = false;
	bool render = false;

	float gameTime = 0;
	int frames = 0;
	int gameFPS = 0;

	/*Project Variables*/
	
	int state = -1;
//	ALLEGRO_SAMPLE *boom = NULL;
//	ALLEGRO_SAMPLE *song = NULL;
	
	ALLEGRO_SAMPLE *step = NULL;

	/*Allegro Variables*/
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_FONT *font18 = NULL;
	ALLEGRO_BITMAP *splashImage = NULL;

	ALLEGRO_BITMAP *borderImage = NULL;

	/*Init Allegro and Display*/
	if (!al_init())
		return -1;

	display = al_create_display(WIDTH, HEIGHT);

	if (!display)
		return -1;

	/*Addons Install*/

	al_init_primitives_addon();
	al_install_keyboard();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_image_addon();
	al_install_audio();
	al_init_acodec_addon();

	/*Project Init*/
	InitNew();
	
	fadeImage = al_load_bitmap("./Pixelart/fade.png");
	splashImage = al_load_bitmap("./Pixelart/splash.png");
	borderImage = al_load_bitmap("./Pixelart/border.png");

	font18 = al_load_font("./Data/SpecialElite.ttf", 18, 0);
	al_reserve_samples(15);
	
	door = al_load_sample("./Data/door.ogg");
	step = al_load_sample("./Data/step.ogg");
	//boom = al_load_sample("boom.ogg");
	//song = al_load_sample("song.ogg");
	
	//songInstance = al_create_sample_instance(song);
	//al_set_sample_instance_playmode(songInstance, ALLEGRO_PLAYMODE_LOOP);

	//al_attach_sample_instance_to_mixer(songInstance, al_get_default_mixer());
	
	/*Timer Init and Startup*/

	srand(time(NULL));

	timer = al_create_timer(1.0 / 60);
	event_queue = al_create_event_queue();

	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_display_event_source(display));

	al_start_timer(timer);

	//debug stuff:
	double fps = 0;
	int frames_done = 0;
	double old_time = al_get_time();
	
	while (!done)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

			if (ev.type == ALLEGRO_EVENT_TIMER)
			{
				render = true;

				if (fade > 15)
					fade -= 2;
				else
					fade = 0;

				/*Update FPS?!*/
				frames++;
				if (al_current_time() - gameTime >= 0.5)
				{
					gameTime = al_current_time();
					gameFPS = frames;
					frames = 0;
					if (l1->player->IsMoving())
						al_play_sample(step, 0.5, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
				}
				if (keys[SHIFT])
					l1->player->SetSpeed(3);
				else
					l1->player->SetSpeed(baseSpeed);

				if (keys[UP])
					l1->player->Move(UP);
				if (keys[DOWN])
					l1->player->Move(DOWN);
				if (keys[LEFT])
					l1->player->Move(LEFT);
				if (keys[RIGHT])
					l1->player->Move(RIGHT);


				if (!keys[UP] && !keys[DOWN])
					l1->player->ResetAnimation(1);
				if (!keys[RIGHT] && !keys[LEFT])
					l1->player->ResetAnimation(0);

				
				if (gameState == PLAYING || gameState == CINEMATIC)
				{
					l1->Collide();
					l1->Update();
					
					if (cin > 0 && gameState == CINEMATIC)
					{
						l1->ci->Update(cin);
						cin--;
					}
	/*				if (l1->player->IsMoving() && !stepping)
					{
						stepping = true;
						al_play_sample(step, 0.5, 0, 1, ALLEGRO_PLAYMODE_LOOP, &stepID);				
					}
					else
					{
						//if (stepID)
						stepping = false;
						al_stop_sample(&stepID);
					}*/

					if (l1->player->GetHealth() <= 0 && gameState == PLAYING)
					{
						ChangeState(LOST);
					}
					l1->CleanUp();

					/*Camera position - scroll with player until we get to the edge of the map*/

					int cx = l1->player->GetX() > WIDTH / 2  ? l1->player->GetX() - WIDTH / 2 : 0;
					cx = l1->player->GetX() < (l1->layerWidth * tileSize - WIDTH / 2) ? cx : l1->layerWidth  * tileSize - WIDTH;

					int cy = l1->player->GetY() > HEIGHT / 2 ? l1->player->GetY() - HEIGHT / 2 : 0;
					cy = l1->player->GetY() < (l1->layerHeight * tileSize - HEIGHT/ 2) ? cy : l1->layerHeight * tileSize - HEIGHT;

					c->Update(cx, cy);

				}
			}
			else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
			{
				switch (ev.keyboard.keycode)
				{
				case ALLEGRO_KEY_ESCAPE:
					if (gameState == TITLE)
						done = true;
					else if (gameState == DIALOGUE || gameState == CINEMATIC)
					{}
					else
						ChangeState(TITLE);
					break;
				case ALLEGRO_KEY_W:
					if (gameState == PLAYING)
					{
						keys[DOWN] = false, keys[RIGHT] = false, keys[LEFT] = false;
						keys[UP] = true;
					}
					if (gameState == DIALOGUE)
						l1->di->MoveChoice(0);
					break;
				case ALLEGRO_KEY_S:
					if (gameState == PLAYING)
					{
						keys[UP] = false, keys[RIGHT] = false, keys[LEFT] = false;
						keys[DOWN] = true;
					}
					if (gameState == DIALOGUE)
						l1->di->MoveChoice(1);
					break;
				case ALLEGRO_KEY_A:
					if (gameState == PLAYING)
					{
						keys[DOWN] = false, keys[RIGHT] = false, keys[UP] = false;
						keys[LEFT] = true;
					}
					break;
				case ALLEGRO_KEY_D:
					if (gameState == PLAYING)
					{
						keys[DOWN] = false, keys[UP] = false, keys[LEFT] = false;
						keys[RIGHT] = true;
					}
					break;
				case ALLEGRO_KEY_Z:
					debug = !debug;
					break;
				case ALLEGRO_KEY_E:
					if (gameState == PLAYING)
					{
						l1->NewDialogue(c->GetX(), c->GetY(), l1->player->InventoryTree());
						ChangeState(DIALOGUE);
					}
					break;
				case ALLEGRO_KEY_LSHIFT:
					keys[SHIFT] = true;
					break;
				case ALLEGRO_KEY_SPACE:
					keys[SPACE] = true;
					if (gameState == DIALOGUE)
					{
						int ch = l1->di->MakeChoice();
						if (ch == NONE)
						{
							//l1->interacting->SetTree(l1->di->GetTree());
							ChangeState(PLAYING);
						}
						else if (ch == NEXT)
						{

						}

						else if (ch == ACTION)
						{
							DialogueTree *d = l1->di->GetTree();
							for (int i = 0; i < 20; i++)
							{
								std::string text = d->choices[i];
								DoAction(text);
							}
							if (gameState == DIALOGUE)
								ChangeState(PLAYING);
						}
					}
					else if (gameState == TITLE)
					{
						ChangeState(PLAYING);
					}
					else if (gameState == PLAYING)
					{
						if (l1->CheckInteract())
						{
							std::cout << "Name: " << l1->interacting->GetName() << "\n";
							l1->NewDialogue(c->GetX(), c->GetY());
							ChangeState(DIALOGUE);
						}
					}
					else if (gameState == LOST)
					{
						ChangeState(TITLE);
					}
					break;
				}
			}
			else if (ev.type == ALLEGRO_EVENT_KEY_UP)
			{
				switch (ev.keyboard.keycode)
				{
				case ALLEGRO_KEY_ESCAPE:
					//done = true;
					break;
				case ALLEGRO_KEY_W:
					if (gameState == PLAYING)
						keys[UP] = false;
					break;
				case ALLEGRO_KEY_S:
					if (gameState == PLAYING)
						keys[DOWN] = false;
					break;
				case ALLEGRO_KEY_A:
					if (gameState == PLAYING)
						keys[LEFT] = false;
					break;
				case ALLEGRO_KEY_D:
					if (gameState == PLAYING)
						keys[RIGHT] = false;
					break;
				case ALLEGRO_KEY_SPACE:
					keys[SPACE] = false;
					break;
				case ALLEGRO_KEY_LSHIFT:
					keys[SHIFT] = false;
					break;
				}
			}
			else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			{
				done = true;
			}

			if (render && al_is_event_queue_empty(event_queue))
			{
				render = false;

				if (gameState == TITLE)
				{
					//al_draw_text(font18, al_map_rgb(100, 200, 120), WIDTH / 2, HEIGHT / 2, ALLEGRO_ALIGN_CENTER, "Ignited, the game.");
					al_draw_bitmap(splashImage, 0, 0, 0);
					
					al_draw_text(font18, al_map_rgb(150, 50, 0), WIDTH / 2, 7 * HEIGHT / 8, ALLEGRO_ALIGN_CENTER, "Press Space to BEGIN");

					//Fade, if applicable
					al_draw_tinted_bitmap(fadeImage, al_map_rgba(0, 0, 0, fade), 0, 0, 0);

				}
				else if (gameState == LOST)
				{
					al_draw_text(font18, al_map_rgb(100, 0, 0), WIDTH / 2, HEIGHT / 4, ALLEGRO_ALIGN_CENTER, "You LOST.  Press SPACE for menu.");


					//Fade, if applicable
					al_draw_tinted_bitmap(fadeImage, al_map_rgba(0, 0, 0, fade), 0, 0, 0);

				}
				else if (gameState == END)
				{
					al_draw_text(font18, al_map_rgb(200, 180, 160), WIDTH / 2, HEIGHT / 3, ALLEGRO_ALIGN_CENTER, "To be continued...");

					al_draw_text(font18, al_map_rgb(200, 180, 160), WIDTH / 2, 2* HEIGHT / 3, ALLEGRO_ALIGN_CENTER, "(That's all there is for now...");
				}
				else if (gameState == PLAYING || gameState == DIALOGUE || gameState == CINEMATIC)
				{
					/*RENDER HERE*/

					//SHAKE, if applicable:
					if (shake >= 0)
					{
						ScreenShake();
						shake--;
					}


					c->Render();
					l1->Render();
					//ci->Render();
					if (gameState == DIALOGUE)
						l1->di->Render();

					if (debug)
					{
						double game_time = al_get_time();
						if (game_time - old_time >= 1.0) {
							fps = frames_done / (game_time - old_time);
							frames_done = 0;
							old_time = game_time;
						}
						frames_done++;
						al_draw_textf(font18, al_map_rgb(255, 230, 100), 10 + c->GetX(), 10 + c->GetY(), 0, "FPS: %f", fps);
					}

					//Fade, if applicable
					al_draw_tinted_bitmap(fadeImage, al_map_rgba(0, 0, 0, fade), c->GetX(), c->GetY(), 0);
					al_draw_bitmap(borderImage, c->GetX(), c->GetY(), 0);
				}
				
				al_flip_display();
				al_clear_to_color(al_map_rgb(0, 0, 0));
			}
		}
	
		/*
	for (iter = objects.begin(); iter != objects.end();)
	{
		(*iter)->Destroy();
		delete(*iter);
		iter = objects.erase(iter);
	}
	*/
//	al_destroy_bitmap(wallImage);
	//al_destroy_sample(boom);
	//al_destroy_sample(shot);
	//al_destroy_sample(song);
	//al_destroy_sample_instance(songInstance);

	l1->Destroy();
	al_destroy_timer(timer);
	al_destroy_event_queue(event_queue);
	al_destroy_display(display);

	return 0;
}

void __cdecl TakeLife()
{
//	ship->LoseLife();
}

void __cdecl ScorePoint()
{
//	ship->AddPoint();
}

void GoToLevel(int newLevel, int toX, int toY, bool play)
{
	fade = 255;
	if (play)
		al_play_sample(door, 0.5, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
	Player *p = l1->player;
	l1 = levels[newLevel];
	l1->player->SetX(toX);
	l1->player->SetY(toY);
	l1->player->SetInventory(p->GetInventory());
	
}

void ChangeState(int newState)
{
	if (newState == END)
	{
		fade = 255;
	}
	else if (newState == DIALOGUE || newState == MENU || gameState == DIALOGUE || newState == CINEMATIC || gameState == CINEMATIC)
	{
		//fade = 0;
	}
	else
		fade = 255;

	if (gameState == TITLE)
	{
		if (newState == PLAYING)
		{
			InitNew();
	//		al_play_sample_instance(songInstance);
		}
	}

	if (newState == DIALOGUE)
	{
		shake = 0;

		int cx = l1->player->GetX() > WIDTH / 2 ? l1->player->GetX() - WIDTH / 2 : 0;
		cx = l1->player->GetX() < (l1->layerWidth * tileSize - WIDTH / 2) ? cx : l1->layerWidth  * tileSize - WIDTH;

		int cy = l1->player->GetY() > HEIGHT / 2 ? l1->player->GetY() - HEIGHT / 2 : 0;
		cy = l1->player->GetY() < (l1->layerHeight * tileSize - HEIGHT / 2) ? cy : l1->layerHeight * tileSize - HEIGHT;

		c->Update(cx, cy);
	}


	if ((gameState == PLAYING || gameState == DIALOGUE) && newState == CINEMATIC)
	{
		cin = 300;
		l1->player->ResetAnimation(0);
		l1->player->ResetAnimation(1);
	}
	else
		cin = 0;

	gameState = newState;

	if (gameState == TITLE || gameState == LOST || gameState == END)
	{
		c->Destroy();
	}

}

void InitNew()
{
	for (int i = 0; i < 7; i++)
	{
/*		if (levels[i] != NULL)
		{
			std::cout << "beforeL " << levels[i]->player->GetY() << " ?\n";
			levels[i]->Destroy();
			delete(levels[i]);

		}*/
		levels[i] = new Level(ls[i], GoToLevel, DoAction, ChangeState);
	}
	l1 = levels[0];//new Level("test2.tmx", GoToLevel);
	c = new Camera();
	for (int i = 0; i < 6; i++)
	{
		keys[i] = false;
	}
	/*ci = new Cinematic(192, 192, "cinematic.txt", DoAction, ChangeState);
	l1->AddObject(ci);*/
}

void ScreenShake()
{
	c->SetX(c->GetX() - rand() % 4);
	c->SetY(c->GetY() - rand() % 4);
}

void DoAction(std::string text)
{
	if (text.compare("GET") == 0)
	{
		Item *it = dynamic_cast<Item*> (l1->interacting);

		if (it != NULL)
		{
			l1->RemoveObject(l1->interacting);
			it->SetTree();
			std::cout << it->GetTree()->text;
			l1->player->AddItem(it);
			l1->interacting = NULL;
		}
	}
	else if (text.compare("DROP") == 0)
	{
		GameObject *o = l1->player->DropItem(l1->di->GetChosen());
		if (o)
		{
			Item *it = dynamic_cast<Item*> (o);
			it->ResetTree();
			o->SetX(tileSize * (l1->player->GetX() / tileSize));
			o->SetY(tileSize * (l1->player->GetY() / tileSize));
			l1->AddObject(o);
		}
	}
	else if (text.compare("USE") == 0)
	{
		l1->CheckInteract();
		if (l1->interacting)
		{
			GameObject *o = l1->player->GetItem(l1->di->GetChosen());
			if (o->DoActions(l1->interacting))
			{
				l1->player->DropItem(l1->di->GetChosen());
				o->Destroy();
			}
		}
	}
	else if (text.compare(0, 8, "CHANGE_D") == 0)
	{
		std::string data = text.substr(8);
		int k = data.find_first_of(":", 0);
		std::string actOn = data.substr(0, k);
		std::string actData = data.substr(k + 1);
		l1->ChangeData(actOn, actData);
	}
	else if (text.compare(0, 8, "CHANGE_I") == 0)
	{
		std::string data = text.substr(8);
		int k = data.find_first_of(":", 0);
		std::string actOn = data.substr(0, k);
		std::string actData = data.substr(k + 1);
		l1->ChangeImage(actOn, actData);
	}
	else if (text.compare(0, 5, "SHAKE") == 0)
	{
	//	std::cout << "SHAKE \n";
		shake = 40;
	}
	else if (text.compare(0, 3, "END") == 0)
	{
	//	std::cout << "END \n";
		l1->player->ResetAnimation(0);
		l1->player->ResetAnimation(1);
		l1->ci->Destroy();
		cin = 0;
		ChangeState(PLAYING);
	}
	else if (text.compare(0, 4, "MOVE") == 0)
	{
	//	std::cout << "MOVE \n";
		int di = atoi(text.substr(4, 1).c_str());
		std::string objName = text.substr(5);
		if (objName.compare("Player") == 0)
		{
			keys[UP] = false, keys[DOWN] = false, keys[RIGHT] = false, keys[LEFT] = false;
			switch (di)
			{
			case UP:
				keys[UP] = true;
				break;
			case DOWN:
				keys[DOWN] = true;
				break;
			case RIGHT:
				keys[RIGHT] = true;
				break;
			case LEFT:
				keys[LEFT] = true;
				break;
			case SHIFT:
				keys[SHIFT] = true;
				break;
			}
		}
		else
		{
			GameObject *o = l1->GetObject(objName);
			NPC *n = dynamic_cast<NPC *> (o);
			if (n)
			{
				n->Move(di);
				if (di <= 1)
					n->ResetAnimation(0);
				else
					n->ResetAnimation(1);
			}
		}
	}
	else if (text.compare(0, 4, "STOP") == 0)
	{
	//	std::cout << "STOP \n";
		std::string objName = text.substr(4);
		if (objName.compare("Player") == 0)
		{
			l1->player->ResetAnimation(0);
			l1->player->ResetAnimation(1);
			for (int i = 0; i < 6; i++)
			{
				keys[i] = false;
			}
		}
		else
		{
			GameObject *o = l1->GetObject(objName);
			NPC *n = dynamic_cast<NPC *> (o);
			if (n)
			{
				std::cout << n->GetName() << "\n";
				n->ResetAnimation(0);
				n->ResetAnimation(1);
			}
		}
	}
	else if (text.compare(0, 8, "DIALOGUE") == 0)
	{
	//	std::cout << "set dialogue \n";
		std::string objName = text.substr(8);
		if (l1->SetInteract(objName))
		{
			l1->NewDialogue(c->GetX(), c->GetY());
			ChangeState(DIALOGUE);
		}
		else if (objName.compare("Player") == 0)
		{
			l1->interacting = l1->player;
			l1->NewDialogue(c->GetX(), c->GetY());
			ChangeState(DIALOGUE);
		}
		else
			std::cout << "failure!\n";
	}
	else if (text.compare(0, 7, "COLLIDE") == 0)
	{
		std::string objName = text.substr(7);
		GameObject *o = l1->GetObject(objName);
		if (o)
		{
			o->Collided(l1->player);
		}
	}
	else if (text.compare(0, 6, "CREATE") == 0)
	{
		int i = text.find_first_of(",", 0);
		int j = text.find_first_of(",", i + 1);
		int k = text.find_first_of(",", j + 1);
		int l = text.find_first_of(",", k + 1);
		int m = text.find_first_of(",", l + 1);
		int n = text.find_first_of(",", m + 1);
		int o = text.find_first_of(",", n + 1);
		int p = text.find_first_of(",", o + 1);
		int x = atoi(text.substr(6, i - 6).c_str());
		if (x == -1)
			x = l1->player->GetX();
		int y = atoi(text.substr(i + 1, j - i).c_str());
		if (y == -1)
			y = l1->player->GetY();
		std::string type = text.substr(j + 1, k - j);
		std::string objName = text.substr(k + 1, l - (k + 1));
		std::string imageFile = text.substr(l + 1, m - (l + 1));
		std::string dataFile = text.substr(m + 1, n - (m + 1));
		int velX = atoi(text.substr(n + 1, o - (n + 1)).c_str());
		int velY = atoi(text.substr(o + 1, p - (o + 1)).c_str());
		int frames = atoi(text.substr(p + 1).c_str());

		if (type.compare(0, 6, "HAZARD") == 0)
		{
			Hazard *h = new Hazard(x, y, imageFile);
			h->SetName(objName);
			l1->AddObject(h);
		}

		if (type.compare(0, 2, "BG") == 0)
		{
			Background *bg = new Background(1, x, y, imageFile);
			l1->AddObject(bg);
		}

		if (type.compare(0, 6, "EFFECT") == 0)
		{
			Effect *ef = new Effect(x, y, velX, velY, frames, imageFile);
			l1->AddObject(ef);

		}

		//more to follow (all objects, potentially)
	}

	else if (text.compare(0, 9, "CINEMATIC") == 0)
	{
		std::string dataFile = text.substr(9);

		Cinematic *cin = new Cinematic(0, 0, dataFile, DoAction, ChangeState);
		l1->ci = cin;
		ChangeState(CINEMATIC);
	}

	else if (text.compare(0, 8, "GAMEOVER") == 0)
	{
		ChangeState(END);
	}

	else if (text.compare(0, 7, "ANIMATE") == 0)
	{
		int k = text.find_first_of(":", 0);
		std::string objName = text.substr(7, k);
		int frames = atoi(text.substr(k + 1).c_str());
		if (objName.compare("Player") == 0)
		{
			l1->player->Animate(frames);
		}
		else
		{
			GameObject *o = l1->GetObject(objName);
			if (o)
			{
				o->Animate(frames);
			}
		}
	}

	//Go to level, play music/sound, etc.

	else if (text.compare(0, 4, "GOTO") == 0)
	{
		int k = text.find_first_of(",", 0);
		int j = text.find_first_of(",", k + 1);
		int tolevel = atoi(text.substr(4, k).c_str());
		int x = atoi(text.substr(k + 1, j).c_str());
		int y = atoi(text.substr(j + 1).c_str());
		ChangeState(PLAYING);
		GoToLevel(tolevel, x, y, false);
	}

	else if (text.compare(0, 4, "PLAY") == 0)
	{
		// to do here...
	}

}